// Shoot Them Up Game. All Rights Reserved


#include "Weapon/STURifleWeapon.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Weapon/Components/STUWeaponFXComponent.h"  
#include "NiagaraComponent.h"
#include "Engine/DamageEvents.h"
#include "NiagaraFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"


ASTURifleWeapon::ASTURifleWeapon()
{
    // Створює компонент для візуальних ефектів зброї (наприклад, спалахи, сліди пострілу) як підоб'єкт за замовчуванням.
    // Це дозволяє автоматично ініціалізувати та керувати ефектами для цієї зброї.
    WeaponFXComponent = CreateDefaultSubobject<USTUWeaponFXComponent>("WeaponFXComponent");
}

void ASTURifleWeapon::BeginPlay()
{
    Super::BeginPlay();

	check(WeaponFXComponent);
}

void ASTURifleWeapon::StartFire()
{
    InitFX();
    GetWorldTimerManager().SetTimer(ShotTimerHandle, this, &ASTURifleWeapon::MakeShot, TimeBetweenShots, true);
    MakeShot();
}

void ASTURifleWeapon::StopFire()
{
    GetWorldTimerManager().ClearTimer(ShotTimerHandle);
    SetFXActive(false);
}

void ASTURifleWeapon::MakeShot()
{
   
	UE_LOG(LogTemp, Display, TEXT("Make Shot!")); 
    if (!GetWorld() || IsAmmoEmpty())
    {
		StopFire();
        return;
    }
   
    FVector TraseStart, TraseEnd;
    if (!GetTraseData(TraseStart, TraseEnd))
    {
        StopFire();
        return;
    }
   

    FHitResult HitResult;
    MakeHit(HitResult, TraseStart, TraseEnd);

	FVector TraseFXEnd = TraseEnd;

    if (HitResult.bBlockingHit)
    {
		TraseFXEnd = HitResult.ImpactPoint;
        MakeDamage(HitResult);
       	WeaponFXComponent->PlayImpactFX(HitResult);
    }
	SpawnTraseFX(GetMuzzleWorldLocation(), TraseFXEnd);
	DecreaseAmmo();
}

bool ASTURifleWeapon::GetTraseData(FVector &TraseStart, FVector &TraseEnd) const
{
    FVector WievLocation;
    FRotator ViewRotation;
    if (!GetPlayerViewPoint(WievLocation, ViewRotation))
        return false;

    TraseStart = WievLocation;
    const auto HalfRad = FMath::DegreesToRadians(1.5f); // 5 degrees cone
    const FVector ShootDirection = FMath::VRandCone(ViewRotation.Vector(), HalfRad);
    TraseEnd = TraseStart + ShootDirection * TraceMaxDistance;
    return true;
}

void ASTURifleWeapon::MakeDamage(const FHitResult& HitResult)
{
    const auto DamagedActor = HitResult.GetActor();
    if (!DamagedActor)
        return;
    FPointDamageEvent PointDamageEvent;
	PointDamageEvent.HitInfo = HitResult;
    DamagedActor->TakeDamage(DamageAmount, PointDamageEvent, GetController(), this);
    
}

void ASTURifleWeapon::InitFX()
{
    if(!MuzzleFXComponent)
    {
        MuzzleFXComponent = SpawnMuzzleFX();
        MuzzleFXComponent->SetTickGroup(TG_PostUpdateWork); // add tick group for correct work with camera shake
	}
    if(!FireAudioComponent)
    {
        FireAudioComponent = UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
	}
    SetFXActive(true);
}
void ASTURifleWeapon::SetFXActive(bool IsActive)
{
    if(MuzzleFXComponent)
    {
        if(IsActive)
        {
			MuzzleFXComponent->ResetSystem(); // Скидає систему ефектів, щоб вона могла відтворюватися з початку при кожному пострілі.
		}
        MuzzleFXComponent->SetPaused(!IsActive);
		MuzzleFXComponent->SetVisibility(IsActive, true);
	}
    if(FireAudioComponent)
    {
		IsActive ? FireAudioComponent->Play() : FireAudioComponent->Stop();
	}
}

void ASTURifleWeapon::SpawnTraseFX(const FVector& TraseStart, const FVector& TraseEnd)
{
	const auto TraseFXComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), TraseFX, TraseStart);
	if (TraseFXComponent)
	{
		TraseFXComponent->SetVariableVec3(TraseTargetName, TraseEnd);
	}
}
AController* ASTURifleWeapon::GetController() const
{
    const auto Pawn = Cast<APawn>(GetOwner());
    return Pawn ? Pawn->GetController() : nullptr;
}
void ASTURifleWeapon::Zoom(bool Enabled)
{
	const auto Contrtoller = Cast<APlayerController>(GetController());
	if (!Contrtoller || !Contrtoller->PlayerCameraManager) return;

    if(Enabled)
    {
        DefaultCameraFOV = Contrtoller->PlayerCameraManager->GetFOVAngle();
	}
    // Встановлює поле зору (FOV) камери гравця для ефекту "прицілювання".
    // Створюється інтервал FOV від 50 до 90 градусів: 50 — прицілювання, 90 — стандартний режим.
  	Contrtoller->PlayerCameraManager->SetFOV(Enabled ? FOVZoomAngle : DefaultCameraFOV);
}