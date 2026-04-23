// Shoot Them Up Game. All Rights Reserved


#include "UI/STUPauseWidget.h"
#include "GameFramework/GameModeBase.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Animation/WidgetAnimation.h"
#include "Kismet/GameplayStatics.h"	




void USTUPauseWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();


	if (ClearPauseButton)
	{
		ClearPauseButton->OnClicked.AddDynamic(this, &USTUPauseWidget::OnClearPause);
	}
	if(SliderMasterVolume)
	{
		SliderMasterVolume->OnValueChanged.AddDynamic(this, &USTUPauseWidget::OnVolumeChange);
		// Додає модифікатор мікшера звуку MasterSoundMix до поточного світу, 
		// дозволяючи змінювати глобальні аудіо-параметри (наприклад, гучність) під час паузи
		UGameplayStatics::PushSoundMixModifier(GetWorld(), MasterSoundMix);
	}
	if(GoToVolume)
	{
		GoToVolume->OnClicked.AddDynamic(this, &USTUPauseWidget::OnVolumeClicked);
	}


}
void USTUPauseWidget::OnClearPause()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->ClearPause(); // Clear the pause state of the game

}
void USTUPauseWidget::OnVolumeChange(float Value)
{
	UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MasterSoundMix, MasterSoundClass, Value, 1.0f, 0.0f);

}
void USTUPauseWidget::OnVolumeClicked()
{

	if (!SliderMasterVolume) return;

	bVolumeVisible = !bVolumeVisible; // Toggle the visibility state of the volume slider
	SliderMasterVolume->SetVisibility(bVolumeVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed); // Set the visibility of the volume slider based on the current state
}