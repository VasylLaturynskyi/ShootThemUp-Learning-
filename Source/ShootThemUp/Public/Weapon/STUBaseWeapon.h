// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STUCoreType.h"
#include "STUBaseWeapon.generated.h"


class USkeletalMeshComponent;
class UNiagaraComponent;
class UNiagaraSystem;
class USoundCue;

UCLASS()
class SHOOTTHEMUP_API ASTUBaseWeapon : public AActor
{
    GENERATED_BODY()

  public:
    ASTUBaseWeapon();

	FOnClipEmptySignature OnClipEmpty;  
    virtual void StartFire();
    virtual void StopFire();

    void ChangeClip();
	bool CanReload() const;
   

    FWeaponUIData GetUIData() const { return UIData; }
    FAmmoData GetAmmoData() const { return CurrentAmmo; }

	bool TryToAddAmmo(int32 ClipsAmount);
    bool IsAmmoEmpty() const;
    bool IsAmmoFull() const;

	virtual void Zoom(bool Enabled) {}

  protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USkeletalMeshComponent *WeaponMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    FName MuzzleSocketName = "MuzzleSocket";

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    float TraceMaxDistance = 5000.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
    FAmmoData DefaultAmmo{15, 10, false};

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FWeaponUIData UIData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VFX")
	UNiagaraSystem* MuzzleFX;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
    USoundCue* FireSound;

    virtual void BeginPlay() override;
    virtual bool GetTraseData(FVector &TraseStart, FVector &TraseEnd) const;
    virtual void MakeShot();
	

    bool GetPlayerViewPoint(FVector & VievLocation, FRotator & VievRotation) const;
    FVector GetMuzzleWorldLocation() const;
    
    void MakeHit(FHitResult &HitResult, const FVector &TraseStart, const FVector &TraseEnd);
    void MakeDamage(const FHitResult &HitResult);

	// Ammo management
	void DecreaseAmmo();
    bool IsCliosEmpty() const;
	void LogAmmo();
	

	UNiagaraComponent* SpawnMuzzleFX();
 

private:
    FAmmoData CurrentAmmo;
    
};