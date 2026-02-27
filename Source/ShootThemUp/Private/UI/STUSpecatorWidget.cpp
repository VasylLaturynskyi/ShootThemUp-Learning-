// Shoot Them Up Game. All Rights Reserved


#include "UI/STUSpecatorWidget.h"
#include "STUUtils.h"
#include "Components/STURespawnComponent.h"


bool USTUSpecatorWidget::GetRespawnTime(int32& CountDownTime) const
{
	const auto RespawnComponent = STUUtils::GetSTUPlayerComponent<USTURespawnComponent>(GetOwningPlayer());
	if (!RespawnComponent || !RespawnComponent->IsRespawnInProgress()) return false;

	CountDownTime = RespawnComponent->GetRespawnCountDown();
	return true;
}