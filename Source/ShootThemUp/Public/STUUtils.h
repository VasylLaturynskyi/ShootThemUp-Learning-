#pragma once
#include "Player/STUPlayerState.h"

class STUUtils
{

public:
	template<typename T>
	static T* GetSTUPlayerComponent(AActor* PlayerActor)
	{
		if (!PlayerActor) return nullptr;

		const auto Component = PlayerActor->GetComponentByClass(T::StaticClass());
		return Cast<T>(Component);
	}
	bool static AreEnemies(AController* Controller1, AController* Controller2)
	{
		if (!Controller1 || !Controller2 || Controller1 == Controller2) return false;

		const auto PlayerState1 = Cast<ASTUPlayerState>(Controller1->PlayerState);
		const auto PlayerState2 = Cast<ASTUPlayerState>(Controller2->PlayerState);

		return PlayerState1 && PlayerState2 && PlayerState1->GetTeamID() != PlayerState2->GetTeamID();
	}
	// Перетворює ціле число (int32) у текстовий формат FText для використання в UI або інших текстових полях Unreal Engine.
	// Використовує FText::FromString для створення FText з рядка, отриманого через FString::FromInt(Number).
	static FText TextFromInt(int32 Number) { return FText::FromString(FString::FromInt(Number)); }
};
