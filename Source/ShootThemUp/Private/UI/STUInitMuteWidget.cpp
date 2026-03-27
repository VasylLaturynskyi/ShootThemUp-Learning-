// Shoot Them Up Game. All Rights Reserved


#include "UI/STUInitMuteWidget.h"

void USTUInitMuteWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
}
void USTUInitMuteWidget::InitMuteWidget(bool IsMute)
{



	UE_LOG(LogTemp, Display, TEXT("IsMute: %s"), IsMute ? TEXT("true") : TEXT("false"));
}