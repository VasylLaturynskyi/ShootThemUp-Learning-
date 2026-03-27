// Shoot Them Up Game. All Rights Reserved


#include "Sound/STUSoundtFunctionLib.h"
#include "Sound/SoundClass.h"


DEFINE_LOG_CATEGORY_STATIC(LogSTUSoundFunctionLib, All, All);
void USTUSoundtFunctionLib::SetSoundClassVolume(USoundClass* SoundClass, float Volume)
{
	if (!SoundClass) return;
	
	SoundClass->Properties.Volume = FMath::Clamp(Volume, 0.0f, 1.0f);
	UE_LOG(LogSTUSoundFunctionLib, Display, TEXT("Sound class volume was changed: %s = %f"), *SoundClass->GetName(), SoundClass->Properties.Volume);
}
void USTUSoundtFunctionLib::ToggleSoundClassVolume(USoundClass* SoundClass)
{
	if (!SoundClass) return;
	const auto NewVolume = SoundClass->Properties.Volume > 0.0f ? 0.0f : 1.0f;
	SetSoundClassVolume(SoundClass, NewVolume);
}