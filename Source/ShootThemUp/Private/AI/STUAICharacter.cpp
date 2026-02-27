// Shoot Them Up Game. All Rights Reserved


#include "AI/STUAICharacter.h"
#include "AI/STUAIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/STUAIWeaponComponent.h"
#include "BrainComponent.h"
#include "Components/WidgetComponent.h"
#include "UI/STUHealthBarWidget.h"
#include "Components/STUHealthComponent.h"

ASTUAICharacter::ASTUAICharacter(const FObjectInitializer& ObjInit) : Super(ObjInit.SetDefaultSubobjectClass<USTUAIWeaponComponent>("WeaponComponent"))
{
	

	AutoPossessAI = EAutoPossessAI::Disabled;
	AIControllerClass = ASTUAIController::StaticClass();

	bUseControllerRotationYaw = false;
	if(GetCharacterMovement())
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		GetCharacterMovement()->RotationRate = FRotator(0.0f, 200.0f, 0.0f);
	}
	HealthWidgetComponent = CreateDefaultSubobject<UWidgetComponent>("HealthWidgetComponent");
	HealthWidgetComponent->SetupAttachment(GetRootComponent());
	HealthWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	// Встановлює прапорець, що віджет здоров'я буде малюватися з розміром, який сам віджет вважає бажаним (тобто автоматично підлаштовується під контент).
	HealthWidgetComponent->SetDrawAtDesiredSize(true);
}
void ASTUAICharacter::BeginPlay()
{
	Super::BeginPlay();
	check(HealthWidgetComponent);
}
void ASTUAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthWidgetVisibility();
}
void ASTUAICharacter::OnDeath()
{
	 Super::OnDeath();

	 /*
	  Якщо обидві перевірки пройшли, викликати STUController->BrainComponent->Cleanup().
	      Це очищає/зупиняє поведінкову логіку AI (наприклад Behavior Tree / BrainComponent),
	      щоб після смерті персонажа AI не продовжував працювати або не звертався до вже видалених об'єктів.
	  Мета: уникнути подальших викликів AI-логіки, можливих витоків пам'яті чи звернень до
	      неіснуючого персонажа після його смерті.
	 */
	 const auto STUController = Cast<ASTUAIController>(GetController());
	 if(STUController && STUController->BrainComponent)
	 {
		 STUController->BrainComponent->Cleanup();
	 }
}
void ASTUAICharacter::OnHealthChanged(float Health, float HealthDelta)
{
	Super::OnHealthChanged(Health, HealthDelta);

	const auto HealthBarWidget = Cast<USTUHealthBarWidget>(HealthWidgetComponent->GetUserWidgetObject());
	if (!HealthBarWidget)return;
	HealthBarWidget->SetHealthPercent(HealthComponent->GetHealthPercent());

}
void ASTUAICharacter::UpdateHealthWidgetVisibility()
{
	if (!GetWorld() || !GetWorld()->GetFirstPlayerController() || !GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()) return;
	// Отримуємо позицію гравця у світі.
	// GetFirstPlayerController() повертає контролер першого гравця (PlayerController).
	// GetPawn() повертає поточного персонажа (Pawn), яким керує цей контролер.
	// GetActorLocation() повертає координати цього персонажа у світі.
	const auto PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator()->GetActorLocation();

	// Обчислюємо відстань між гравцем і цим AI-персонажем у світі.
	// FVector::Distance повертає скалярну (числову) відстань між двома точками у 3D-просторі.
	const auto Distance = FVector::Distance(PlayerLocation, GetActorLocation());
	HealthWidgetComponent->SetVisibility(Distance < HealthWidgetVisibilityDistance, true);
}