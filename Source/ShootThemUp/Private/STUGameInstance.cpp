// Shoot Them Up Game. All Rights Reserved


#include "STUGameInstance.h"
#include "Sound/STUSoundtFunctionLib.h"

void USTUGameInstance::ToggleVolume()
{
	// Викликає статичну функцію ToggleSoundClassVolume з бібліотеки STUSoundtFunctionLib,
	USTUSoundtFunctionLib::ToggleSoundClassVolume(MasterSoundClass);

}