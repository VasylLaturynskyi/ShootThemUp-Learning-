// Shoot Them Up Game. All Rights Reserved


#include "Components/STUWeaponComponent.h"
#include "Weapon/STUBaseWeapon.h"
#include "Animations/STUWquipFinishedAnimNotify.h"
#include "Animations/STUReloadFinishedAnimNotify.h"
#include "GameFramework/Character.h"
#include "Animations/AnimUtil.h"



DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

// Оголошує константу WeaponNum, яка визначає максимальну кількість
constexpr static int32 WeaponNum = 2;   

USTUWeaponComponent::USTUWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTUWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(WeaponData.Num() == WeaponNum, TEXT("Our character can hold only %i weapons item"), WeaponNum);

	CurrentWeaponIndex = 0;
    InitAnimation();
    SpawnWeapons();
    EquipWeapon(CurrentWeaponIndex);
}

void USTUWeaponComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	CurrentWeapon = nullptr;
    for(auto Weapon : Weapons)
    {
        if (!Weapon) continue;
		
        Weapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        Weapon->Destroy();
	}
    Weapons.Empty();
	Super::EndPlay(EndPlayReason);
}

void USTUWeaponComponent::SpawnWeapons()
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
  
    if (!Character || !GetWorld())
        return;
    for(auto OneWeaponData : WeaponData)
    {
        auto Weapon = GetWorld()->SpawnActor<ASTUBaseWeapon>(OneWeaponData.WeaponClass);
        if (!Weapon) continue;
		Weapon->OnClipEmpty.AddUObject(this, &USTUWeaponComponent::OnEmptyClip);
        Weapon->SetOwner(Character);
        Weapons.Add(Weapon);

        // Прикріплює зброю (Weapon) до заданого сокета (SocketName) на скелетній сітці персонажа (Mesh).
        AttachWeaponToSocket(Weapon, Character->GetMesh(), WeaponArmorySocketName);
    }
 }

void USTUWeaponComponent::AttachWeaponToSocket(ASTUBaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName)
{
    if (!Weapon || !SceneComponent)
		return;

    FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
    Weapon->AttachToComponent(SceneComponent, AttachmentRules, SocketName);

}

void USTUWeaponComponent::EquipWeapon(int32 WeaponIndex)
{
    if(WeaponIndex < 0 || WeaponIndex >= Weapons.Num())
    {
        UE_LOG(LogWeaponComponent, Warning, TEXT("Invalid weapon index"));
        return;
	}
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character)return;

    if (CurrentWeapon)
    { 
		CurrentWeapon->StopFire();
        AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponArmorySocketName);
    }

	CurrentWeapon = Weapons[WeaponIndex];
	CurrentReloadAnimMontage = WeaponData[WeaponIndex].ReloadAnimMontage;
    const auto CurrentWeaponData = WeaponData.FindByPredicate([&](const FWeaponData& Data) { return Data.WeaponClass == CurrentWeapon->GetClass();});
	CurrentReloadAnimMontage = CurrentWeaponData ? CurrentWeaponData->ReloadAnimMontage : nullptr;
    
    AttachWeaponToSocket(CurrentWeapon, Character->GetMesh(), WeaponEquipSocketName);
    UE_LOG(LogWeaponComponent, Warning, TEXT("EquipAnimInProgress = true"));
	EquipAnimInProgress = true;
	PlayEquipAnimMontage(EquipAnimMontage);
}

void USTUWeaponComponent::StartFire()
{
    if (!CanFire())
        return;
    CurrentWeapon->StartFire();

}

void USTUWeaponComponent::StopFire()
{
    if (!CurrentWeapon)
        return;
    CurrentWeapon->StopFire();
}

void USTUWeaponComponent::NextWeapon()
{
    if(!CanEquip())
		return;
    
// Отримуємо вказівник на персонажа, якому належить цей компонент, шляхом приведення власника (Owner) до типу ACharacter.
    
    ACharacter* Character = Cast<ACharacter>(GetOwner());
 // Збільшує індекс поточної зброї на 1.
 // Якщо індекс перевищує кількість зброї, повертається до 0 (циклічний вибір).
 // Це дозволяє перемикати зброю по колу.
    CurrentWeaponIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	EquipWeapon(CurrentWeaponIndex);
   
}

void USTUWeaponComponent::PlayEquipAnimMontage(UAnimMontage* Animation)
{
    
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if(!Character)return;

	Character->PlayAnimMontage(Animation);
}

void USTUWeaponComponent::InitAnimation()
{
	auto EquipFinishedNotify = AnimUtil::FindNotifyByClass<USTUWquipFinishedAnimNotify>(EquipAnimMontage);
    if (EquipFinishedNotify)
    {

        EquipFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnEquipFinished);
    }
    else
	{
        // це макрос із Unreal Engine, який використовується для виявлення помилок у коді під час розробки.
		checkNoEntry();
	}
 
 
    for (auto OneWeaponData : WeaponData)
    {
        auto ReloadFinishedNotify = AnimUtil::FindNotifyByClass<USTUReloadFinishedAnimNotify>(OneWeaponData.ReloadAnimMontage);
        if (!ReloadFinishedNotify)
        {
            
            // це макрос із Unreal Engine, який використовується для виявлення помилок у коді під час розробки.
            checkNoEntry();
        }
        
        ReloadFinishedNotify->OnNotified.AddUObject(this, &USTUWeaponComponent::OnReloadFinished);
	}
}

void USTUWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComponent)
{
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;
    EquipAnimInProgress = false;
   	

 
}
void USTUWeaponComponent::OnReloadFinished(USkeletalMeshComponent* MeshComponent)
{
    
    ACharacter* Character = Cast<ACharacter>(GetOwner());
    if (!Character || MeshComponent != Character->GetMesh()) return;
    
    ReloadAnimInProgress = false;
    


}
bool USTUWeaponComponent::CanFire() const
{
    return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanEquip() const
{
    return !EquipAnimInProgress && !ReloadAnimInProgress;
}
bool USTUWeaponComponent::CanReload() const
{
   return CurrentWeapon && !EquipAnimInProgress && !ReloadAnimInProgress && CurrentWeapon->CanReload();
}
void USTUWeaponComponent::Reload()
{
    ChangeClip();
}
void USTUWeaponComponent::OnEmptyClip(ASTUBaseWeapon* AmmoEmptyWeapon)
{
	if (!AmmoEmptyWeapon) return;
    if (CurrentWeapon == AmmoEmptyWeapon)
    {
        
        ChangeClip();
    }
    else
    {
        for(const auto Weapon : Weapons)
        {
            if(Weapon == AmmoEmptyWeapon)
            {
                Weapon->ChangeClip();
                
            }
		}
    }
}
void USTUWeaponComponent::ChangeClip()
{
    UE_LOG(LogWeaponComponent, Error, TEXT("перезарядка2"));
    if (!CanReload()) return;
	CurrentWeapon->StopFire();
	CurrentWeapon->ChangeClip();
    ReloadAnimInProgress = true;
    UE_LOG(LogWeaponComponent, Error, TEXT("перезарядка діє"));
    PlayEquipAnimMontage(CurrentReloadAnimMontage);

}
bool USTUWeaponComponent::GetCurrentWeaponUIData(FWeaponUIData& UIData) const
{
    if (CurrentWeapon)
    {
        UIData = CurrentWeapon->GetUIData();
        return true;
    }
    return false;
}
bool USTUWeaponComponent::GetCurrentWeaponAmmoData(FAmmoData& AmmoData) const
{
    if (CurrentWeapon)
    {
        AmmoData = CurrentWeapon->GetAmmoData();
        return true;
    }
    return false;
}
bool USTUWeaponComponent::TryToAddAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType, int32 ClipsAmount)
{
    for(const auto Weapon : Weapons)
    {
        if(Weapon && Weapon->IsA(WeaponType))
        {
            return Weapon->TryToAddAmmo(ClipsAmount);
        }
	}
	return false;
}
bool USTUWeaponComponent::NeedAmmo(TSubclassOf<ASTUBaseWeapon> WeaponType)
{
    for (const auto Weapon : Weapons)
    {
        if (Weapon && Weapon->IsA(WeaponType))
        {
            return !Weapon->IsAmmoFull();
        }
    }
    return false;
}