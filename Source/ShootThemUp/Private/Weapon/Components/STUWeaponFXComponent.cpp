// Shoot Them Up Game. All Rights Reserved


#include "Weapon/Components/STUWeaponFXComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Components/DecalComponent.h"
#include "Sound/SoundCue.h"

void USTUWeaponFXComponent::BeginPlay()
{
	Super::BeginPlay();
	// Додайте тут вашу логіку, якщо потрібно
}


USTUWeaponFXComponent::USTUWeaponFXComponent()
{

	PrimaryComponentTick.bCanEverTick = true;

}

void USTUWeaponFXComponent::PlayImpactFX(const FHitResult& Hit)
{
	auto ImpactData = DefaultImpactData;

if (Hit.PhysMaterial.IsValid())
	{
		const auto PhysMat = Hit.PhysMaterial.Get();
		if(ImpactDataMap.Contains(PhysMat))
		{
			ImpactData = ImpactDataMap[PhysMat];
		}
	}

    //Fix: Cast NiagaraSystem* to UNiagaraSystem* when calling SpawnSystemAtLocation

    UNiagaraFunctionLibrary::SpawnSystemAtLocation(
        GetWorld(),
        ImpactData.NiagaraEffect,
        Hit.ImpactPoint,
        Hit.ImpactNormal.Rotation()
    );
//Niagara
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), 
		ImpactData.NiagaraEffect, 
		Hit.ImpactPoint, 
		Hit.ImpactNormal.
		Rotation());
	

// Decal
	auto DecalComponent = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), 
		ImpactData.DecalData.Material, 
		ImpactData.DecalData.Size, 
		Hit.ImpactPoint, 
		Hit.ImpactNormal.
		Rotation());
	if (DecalComponent)
	{
		DecalComponent->SetFadeOut(ImpactData.DecalData.LifeTime, ImpactData.DecalData.FadeOutTime);
	}

// Sound
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactData.Sound, Hit.ImpactPoint);
}