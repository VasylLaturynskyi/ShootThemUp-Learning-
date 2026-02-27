// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "STUAIPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTTHEMUP_API USTUAIPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
// Повертає найближчого ворога серед акторів, яких бачить AI.
// Реалізація методу буде знаходити всіх акторів, сприйнятих компонентом сприйняття,
// фільтрувати їх як ворогів, і вибирати того, що знаходиться найближче до поточного AI.
	AActor* GetClosestEnemy() const;
};
