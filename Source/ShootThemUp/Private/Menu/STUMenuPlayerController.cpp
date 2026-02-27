// Shoot Them Up Game. All Rights Reserved


#include "Menu/STUMenuPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "STUGameInstance.h"

DEFINE_LOG_CATEGORY_STATIC(LogSTUMenuPlayerController, All, All);


void ASTUMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// Встановлює режим введення лише для UI (користувацького інтерфейсу), щоб гравець міг взаємодіяти тільки з елементами меню, а не з ігровим світом
	SetInputMode(FInputModeUIOnly());
	bShowMouseCursor = true;

	//GetWorld()->GetGameInstance<USTUGameInstance>()->TestString = "Hello from STUMenuPlayerController!";

	// Отримує всі актори типу ACameraActor у світі та зберігає їх у масив Cameras.
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraActor::StaticClass(), Cameras);

	// Якщо знайдено хоча б одну камеру, встановлює її як поточну камеру для перегляду та виводить її ім'я у лог.
	if(Cameras.Num() > 0)
	{
		SetViewTarget(Cameras[0]);
		UE_LOG(LogSTUMenuPlayerController, Warning, TEXT("Camera found: %s"), *Cameras[0]->GetName());
	}
	else
	{
		return;
	}

}