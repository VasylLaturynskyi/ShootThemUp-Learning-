// Shoot Them Up Game. All Rights Reserved


#include "Weapon/STUBaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/DamageEvents.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All);

ASTUBaseWeapon::ASTUBaseWeapon()
{
	 	
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMesh");
	SetRootComponent(WeaponMesh);
}

void ASTUBaseWeapon::BeginPlay()
{
	Super::BeginPlay();
    
    check(WeaponMesh);
    // checkf - це макрос для перевірки умови під час виконання. 
    // Якщо умова невірна, гра аварійно завершується з вказаним повідомленням.
    // Тут перевіряється, що кількість куль за замовчуванням більше нуля.
    checkf(DefaultAmmo.Bullets > 0, TEXT("Bullets count couldn't be less or equal zero"));
	checkf(DefaultAmmo.Clips > 0, TEXT("Clips count couldn't be less or equal zero"));  
	CurrentAmmo = DefaultAmmo;
}

void ASTUBaseWeapon::StartFire()
{
	
}

void ASTUBaseWeapon::StopFire()
{

}

void ASTUBaseWeapon::MakeShot()
{
   
}

bool ASTUBaseWeapon::GetPlayerViewPoint(FVector& VievLocation, FRotator& VievRotation) const
{
	const auto STUCharacter = Cast<ACharacter>(GetOwner());
    if(!STUCharacter) return false;

    if(STUCharacter->IsPlayerControlled())
    {
        const auto Controller = STUCharacter->GetController<APlayerController>();
        if (!Controller)
            return false;
        Controller->GetPlayerViewPoint(VievLocation, VievRotation);
	}
    else
    {
        VievLocation = GetMuzzleWorldLocation();
		VievRotation = WeaponMesh->GetSocketRotation(MuzzleSocketName);
    }

    return true;
}
FVector ASTUBaseWeapon::GetMuzzleWorldLocation() const
{
    return WeaponMesh->GetSocketLocation(MuzzleSocketName);
}
bool ASTUBaseWeapon::GetTraseData(FVector &TraseStart, FVector &TraseEnd) const
{
    FVector WievLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(WievLocation, ViewRotation))
    return false;

    TraseStart = WievLocation;
    const FVector ShootDirection = ViewRotation.Vector();
    TraseEnd = TraseStart + ShootDirection * TraceMaxDistance;
    return true;
}
void ASTUBaseWeapon::MakeHit(FHitResult &HitResult, const FVector &TraseStart, const FVector &TraseEnd)
{
    if (!GetWorld())
        return;
    FCollisionQueryParams CollisionParams;
    CollisionParams.AddIgnoredActor(GetOwner());
	CollisionParams.bReturnPhysicalMaterial = true;

    GetWorld()->LineTraceSingleByChannel(HitResult, TraseStart, TraseEnd, ECollisionChannel::ECC_Visibility, CollisionParams);
}

// Ammo management methods
void ASTUBaseWeapon::DecreaseAmmo()
{ 
    if (CurrentAmmo.Bullets == 0)
    {
        UE_LOG(LogBaseWeapon, Warning, TEXT("Clips is empty"));
        return;
    }
	CurrentAmmo.Bullets--;

    if (IsCliosEmpty() && !IsAmmoEmpty())
    {
		StopFire();
		OnClipEmpty.Broadcast(this);
    }
    
}
bool ASTUBaseWeapon::IsAmmoEmpty() const
{
	return !CurrentAmmo.Infinite && CurrentAmmo.Clips == 0 && IsCliosEmpty();
}
bool ASTUBaseWeapon::IsCliosEmpty() const
{
    return CurrentAmmo.Bullets == 0;
}
void ASTUBaseWeapon::ChangeClip()
{
	
    if(!CurrentAmmo.Infinite)
    {
        if(CurrentAmmo.Clips == 0)
        {
            UE_LOG(LogBaseWeapon, Warning, TEXT("No more clips"));
            return;
		}
        CurrentAmmo.Clips--;
	}
    CurrentAmmo.Bullets = DefaultAmmo.Bullets;
    //UE_LOG(LogBaseWeapon, Display, TEXT("-------Change Clip-------"));
}
bool ASTUBaseWeapon::CanReload() const
{
	return CurrentAmmo.Bullets < DefaultAmmo.Bullets && CurrentAmmo.Clips > 0;
}
void ASTUBaseWeapon::LogAmmo()
{
    // Формуємо рядок з інформацією про поточну кількість патронів та магазинів (або "Infinite" якщо нескінченні)
    FString AmmoInfo = "Ammo" + FString::FromInt(CurrentAmmo.Bullets) + " / " ;
    AmmoInfo += CurrentAmmo.Infinite ? "Infinite" : FString::FromInt(CurrentAmmo.Clips);

    UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *AmmoInfo);
}
bool ASTUBaseWeapon::IsAmmoFull() const
{
	return CurrentAmmo.Clips == DefaultAmmo.Clips && CurrentAmmo.Bullets == DefaultAmmo.Bullets;
}
bool ASTUBaseWeapon::TryToAddAmmo(int32 ClipsAmount)
{
    if(CurrentAmmo.Infinite || IsAmmoFull() || ClipsAmount <= 0)
        return false;

    if(IsAmmoEmpty())
	{
        UE_LOG(LogBaseWeapon, Display, TEXT("Ammo was enpty"));
		CurrentAmmo.Clips = FMath::Clamp(CurrentAmmo.Clips + ClipsAmount, 0, DefaultAmmo.Clips + 1);
		OnClipEmpty.Broadcast(this);
    }
    else if (CurrentAmmo.Clips < DefaultAmmo.Clips)
    {
		const auto NextClipsAmount = CurrentAmmo.Clips + ClipsAmount;
        if (DefaultAmmo.Clips - NextClipsAmount >= 0)
        {
            CurrentAmmo.Clips = NextClipsAmount;
            UE_LOG(LogBaseWeapon, Display, TEXT("Clips were added"));
        }
        else
        {
			CurrentAmmo.Clips = DefaultAmmo.Clips;
			CurrentAmmo.Bullets = DefaultAmmo.Bullets;
            UE_LOG(LogBaseWeapon, Display, TEXT("Ammo is full now"));
        }
    }
    else
    {
		CurrentAmmo.Bullets = DefaultAmmo.Bullets;
        UE_LOG(LogBaseWeapon, Display, TEXT("Bullets were added"));
    }
return true;
}

UNiagaraComponent* ASTUBaseWeapon::SpawnMuzzleFX()
{
    return UNiagaraFunctionLibrary::SpawnSystemAttached(MuzzleFX, WeaponMesh, MuzzleSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);

}
