// Shoot Them Up Game. All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "QueryTest_PickupCouldBeTaken.generated.h"


UCLASS()
class SHOOTTHEMUP_API UQueryTest_PickupCouldBeTaken : public UEnvQueryTest
{
	GENERATED_BODY()
public:
	UQueryTest_PickupCouldBeTaken(const FObjectInitializer& ObjectInitializer);

	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
		
};
