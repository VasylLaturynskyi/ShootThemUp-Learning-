// Shoot Them Up Game. All Rights Reserved


#include "Animations/STUAnimNotify.h"

void USTUAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	// Викликає базову реалізацію Notify, щоб зберегти стандартну поведінку анімованого повідомлення.
	// Це дозволяє розширити або змінити функціонал, не втрачаючи базових можливостей.
	Super::Notify(MeshComp, Animation, EventReference);
	OnNotified.Broadcast(MeshComp);
}			