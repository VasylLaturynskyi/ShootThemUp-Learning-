// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/STUBaseWidget.h"
#include "Sound/SoundMix.h"
#include "Sound/SoundClass.h"
#include "Animation/WidgetAnimation.h"
#include "STUPauseWidget.generated.h"

class UButton;
class USlider;
UCLASS()
class SHOOTTHEMUP_API USTUPauseWidget : public USTUBaseWidget
{
	GENERATED_BODY()
	
public:
	

protected:
	UPROPERTY(meta = (BindWidget))
	USlider* SliderMasterVolume;

	UPROPERTY(meta = (BindWidget))
	UButton* GoToVolume;
		
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundMix* MasterSoundMix;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Sound")
	USoundClass* MasterSoundClass;

	UPROPERTY(meta = (BindWidget)) // Binding the button from the UI
	UButton* ClearPauseButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnClearPause();

	UFUNCTION()
	void OnVolumeChange(float Value);

	UFUNCTION()
	void OnVolumeClicked();

	bool bVolumeVisible = false;
	UWidgetAnimation* VolumeSliderAnim;

};
