// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "STUInitMuteWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUInitMuteWidget : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeOnInitialized() override;
	void InitMuteWidget( bool IsMute);
};
