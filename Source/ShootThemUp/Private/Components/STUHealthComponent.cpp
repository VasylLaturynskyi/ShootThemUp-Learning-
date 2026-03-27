// Shoot Them Up Game. All Rights Reserved


#include "Components/STUHealthComponent.h"
//#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Camera/CameraShakeBase.h"	
#include "STUGameModeBase.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Perception/AISense_Damage.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All)


USTUHealthComponent::USTUHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


void USTUHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	check(MaxHealth > 0);

	SetHealth(MaxHealth);
   
    
	AActor *ComponentOwner = GetOwner();
	if (ComponentOwner)
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &USTUHealthComponent::OnTakeAnyDamage);
		ComponentOwner->OnTakePointDamage.AddDynamic(this, &USTUHealthComponent::OnTakePointDamage);
		ComponentOwner->OnTakeRadialDamage.AddDynamic(this, &USTUHealthComponent::OnTakeRadialDamage);

	}
}

void USTUHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
  
}

void USTUHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy, FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser)
{
	const auto FinalDamage = Damage * GetPointDamageModifier(DamagedActor, BoneName);
	UE_LOG(LogHealthComponent, Display, TEXT("On POINT damage: %f, FINAL damage: %f, bone: %s"), Damage, FinalDamage, *BoneName.ToString());
	ApplyDamage(FinalDamage, InstigatedBy);
}
void USTUHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,FVector Origin, const FHitResult& HitInfo, class AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("On RADIAL damage: %f"), Damage);
	ApplyDamage(Damage, InstigatedBy);
}
void USTUHealthComponent::OnTakeAnyDamage(AActor *DamagedActor, float Damage, const UDamageType *DamageType, AController *InstigatedBy, AActor *DamageCauser) 
{	
	UE_LOG(LogHealthComponent, Display, TEXT("On ANY damage: %f"), Damage);
}
void USTUHealthComponent::ApplyDamage(float Damage, AController* InstigatedBy)
{
	if (Damage <= 0.0f || IsDead() || !GetWorld())
		return;
	SetHealth(Health - Damage);
	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	if (IsDead())
	{
		Killed(InstigatedBy);
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &USTUHealthComponent::HealUpdate, HealUpDateTime, true, HealDelay);
	}

	PlayCameraShake();
	ReportDamageEvent(Damage, InstigatedBy);
}

void USTUHealthComponent::HealUpdate()
{
    SetHealth(Health + HealModitier);

	if (IsHealthFull() && GetWorld())
	{
        GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

void USTUHealthComponent::SetHealth(float NewHealth)
{
	const auto NextHealth = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
	const auto HealthDelta = NextHealth - Health;

	Health = NextHealth;
    OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool USTUHealthComponent::TryToAddHealth(float HealthAmount)
{
	if(IsDead() || IsHealthFull())
		return false;
	SetHealth(Health + HealthAmount);
	return true;
}
bool USTUHealthComponent::IsHealthFull() const
{
	return FMath::IsNearlyEqual(Health, MaxHealth);
}
void USTUHealthComponent::PlayCameraShake()
{
	if (IsDead()) return;

	const auto Player = Cast<APawn>(GetOwner());
	if (!Player) return;

	const auto Controller = Player->GetController<APlayerController>();
	if (!Controller || !Controller->PlayerCameraManager) return;

	Controller->PlayerCameraManager->StartCameraShake(CameraShake);
}
void USTUHealthComponent::Killed(AController* KillerController)
{
	if(!GetWorld()) return;
	// Отримуємо поточний ігровий режим (GameMode) і намагаємось привести його до типу ASTUGameModeBase.
	// Це дозволяє викликати специфічні для цього класу методи, наприклад, для обробки смерті гравця.
	const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());

	if (!GameMode) return;

	const auto Player = Cast<APawn>(GetOwner());
	const auto VictimController = Player ? Player->GetController() : nullptr;

	GameMode->Killed(KillerController, VictimController);
}
float USTUHealthComponent::GetPointDamageModifier(AActor* DamagedActor, const FName& BoneName)
{
	const auto Character = Cast<ACharacter>(DamagedActor);
	if (!Character || !Character->GetMesh() || !Character->GetMesh()->GetBodyInstance(BoneName))return 1.0f;

	// Отримуємо фізичний матеріал (PhysicalMaterial) для конкретної кістки (BoneName) персонажа (Character).
	const auto PhysicalMaterial = Character->GetMesh()->GetBodyInstance(BoneName)->GetSimplePhysicalMaterial();
	if (!PhysicalMaterial || !DamageModifiers.Contains(PhysicalMaterial)) return 1.0f;

	return DamageModifiers[PhysicalMaterial];
}
void USTUHealthComponent::ReportDamageEvent(float Damage, AController* InstigatedBy)
{
	
	if (!InstigatedBy || InstigatedBy->GetPawn() || !GetOwner()) return;
	UAISense_Damage::ReportDamageEvent(GetWorld(), GetOwner(), InstigatedBy->GetPawn(), Damage, InstigatedBy->GetPawn()->GetActorLocation(), GetOwner()->GetActorLocation());
	
}