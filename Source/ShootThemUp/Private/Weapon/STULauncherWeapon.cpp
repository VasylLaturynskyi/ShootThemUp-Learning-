// Shoot Them Up Game. All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"


void ASTULauncherWeapon::StartFire()
{
    
	MakeShot();
    UE_LOG(LogActor, Error, TEXT("нема шоту"));
}

void ASTULauncherWeapon::MakeShot()
{
    
    if (!GetWorld() || IsAmmoEmpty())
        return;
    FVector TraseStart, TraseEnd;
    if (!GetTraseData(TraseStart, TraseEnd))
        return;

    FHitResult HitResult;
    MakeHit(HitResult, TraseStart, TraseEnd);

	const FVector EndPoint = HitResult.bBlockingHit ? HitResult.ImpactPoint : TraseEnd;
	const FVector Direction = (EndPoint - GetMuzzleWorldLocation()).GetSafeNormal();

  // Зазвичай використовується для визначення місця появи снаряда у світі гри
  const FTransform SpawnTransform(FRotator::ZeroRotator, GetMuzzleWorldLocation());
  ASTUProjectile* Projectile = GetWorld()->SpawnActorDeferred<ASTUProjectile>(ProjectileClass, SpawnTransform);
  if(Projectile)
  {
      Projectile->SetShotDirection(Direction);
	  Projectile->SetOwner(GetOwner());
      Projectile->FinishSpawning(SpawnTransform);
  }
  DecreaseAmmo();
  SpawnMuzzleFX();
}