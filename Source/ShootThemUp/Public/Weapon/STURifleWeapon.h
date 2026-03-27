// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Weapon/STUBaseWeapon.h"
#include "STURifleWeapon.generated.h"


class USTUWeaponFXComponent;    
class UNiagaraComponent;
class AudioComponent;



UCLASS()
class SHOOTTHEMUP_API ASTURifleWeapon : public ASTUBaseWeapon
{
	GENERATED_BODY()
	
public:
	ASTURifleWeapon();

	
    virtual void StartFire() override;
    virtual void StopFire() override;
	virtual void Zoom(bool Enabled) override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TimeBetweenShots = 0.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float BulletSpread = 1.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float DamageAmount = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* TraseFX;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "VFX")
	FName TraseTargetName = "TraseTarget";

    UPROPERTY(VisibleAnywhere, Category = "VFX")
	USTUWeaponFXComponent* WeaponFXComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FOVZoomAngle = 50.0f;

	virtual void BeginPlay() override;
    virtual void MakeShot() override;
    virtual bool GetTraseData(FVector &TraseStart, FVector &TraseEnd) const override;

private:
    FTimerHandle ShotTimerHandle;

    UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;

	UPROPERTY()
	UAudioComponent* FireAudioComponent;

    void MakeDamage(const FHitResult& HitResult);
	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraseFX(const FVector& TraseStart, const FVector& TraseEnd);
	
	AController* GetController() const;

	float DefaultCameraFOV = 90.0f;
 
};

