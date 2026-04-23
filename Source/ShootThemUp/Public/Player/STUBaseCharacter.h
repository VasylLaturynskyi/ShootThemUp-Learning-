// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "STUBaseCharacter.generated.h"



class USTUHealthComponent;
class USTUWeaponComponent;
class USoundCue;

    UCLASS()
class SHOOTTHEMUP_API ASTUBaseCharacter : public ACharacter
{
    GENERATED_BODY()

 
  protected:
      ASTUBaseCharacter(const FObjectInitializer& ObjInit);

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    TObjectPtr<USTUHealthComponent> HealthComponent;

    UPROPERTY(VisibleDefaultsOnly, Category = "Components")
    USTUWeaponComponent *WeaponComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathAnimMontage;
    
    UPROPERTY(EditAnywhere, Category = "Damage")
    FVector2D LandedDamageVelocity = FVector2D(500.0f, 1200.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Damage")
    FVector2D LandedDamage = FVector2D(10.0f, 100.0f);

    UPROPERTY(EditDefaultsOnly, Category = "Material")
	FName MaterialColorName = "Paint Color";

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundCue* DeathSound;

    virtual void BeginPlay() override;
    virtual void OnDeath();
    virtual void OnHealthChanged(float Health, float HealthDelta);

  public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
	virtual void TurnOff() override;
    virtual void Reset() override;


    UFUNCTION(BlueprintCallable, Category = "Movement")
    virtual bool IsRunning() const;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    float GetMovementDirection() const;

    UFUNCTION(BlueprintCallable)
    bool IsJumping() const;

	void SetPlayerColor(const FLinearColor& Color);

  private:
  
    UFUNCTION()
    void OnGroupLanded(const FHitResult & Hit);
    
};