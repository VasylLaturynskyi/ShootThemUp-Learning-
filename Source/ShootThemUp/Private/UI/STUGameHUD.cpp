// Shoot Them Up Game. All Rights Reserved


#include "UI/STUGameHUD.h"
#include "Engine/Canvas.h"
#include "STUGameModeBase.h"
#include "UI/STUBaseWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUGameHUD, All, All);

void ASTUGameHUD::DrawHUD()
{
    Super::DrawHUD();

    //DrawCrossHiar();
}
void ASTUGameHUD::BeginPlay()
{
	Super::BeginPlay(); 
	
	GameWidgets.Add(ESTUMatchState::InProgress, CreateWidget<USTUBaseWidget>(GetWorld(), PlayerHUDWidgetClass));
    GameWidgets.Add(ESTUMatchState::Paused, CreateWidget<USTUBaseWidget>(GetWorld(), PauseWidgetClass));
    GameWidgets.Add(ESTUMatchState::GameOver, CreateWidget<USTUBaseWidget>(GetWorld(), GameOverWidgetClass));

    for(auto GameWidgetPair : GameWidgets)
    {
        const auto GameWidget = GameWidgetPair.Value;
		if (!GameWidget) continue;
        
            GameWidget->AddToViewport();
            // Ховає віджет гри з екрану, роблячи його невидимим, але не видаляючи з viewport.
            GameWidget->SetVisibility(ESlateVisibility::Hidden);
        
	}

    if (GetWorld())
    {
        const auto GameMode = Cast<ASTUGameModeBase>(GetWorld()->GetAuthGameMode());
        if(GameMode)
        {
            GameMode->OnMatchStateChanged.AddUObject(this, &ASTUGameHUD::OnMatchStateChanged);
		}
    }
}
void ASTUGameHUD::OnMatchStateChanged(ESTUMatchState State)
{
    // Якщо поточний віджет існує, приховуємо його (робимо невидимим)
    if(CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Hidden);
	}
    // Якщо у мапі GameWidgets є віджет для поточного стану гри, призначаємо його як поточний
    if (GameWidgets.Contains(State))
    {
        CurrentWidget = GameWidgets[State];
    }
    // Якщо поточний віджет існує, показуємо його (робимо видимим)
    if (CurrentWidget)
    {
        CurrentWidget->SetVisibility(ESlateVisibility::Visible);
		CurrentWidget->Show();
    }
	UE_LOG(LogSTUGameHUD, Display, TEXT("Match state changed: %s"), *UEnum::GetValueAsString(State));
}
void ASTUGameHUD::DrawCrossHiar()
{
    const TInterval<float> Center(Canvas->SizeX * 0.5f, Canvas->SizeY * 0.5f);

    const float HalfLineSize = 10.0f;
    const float LineThickness = 2.0f;
    const FLinearColor LineColor = FLinearColor::Green;
    DrawLine(Center.Min - HalfLineSize, Center.Max, Center.Min + HalfLineSize, Center.Max, LineColor, LineThickness);
    DrawLine(Center.Min, Center.Max - HalfLineSize, Center.Min, Center.Max + HalfLineSize, LineColor, LineThickness);
}
