// Shoot Them Up Game. All Rights Reserved


#include "Weapon/STULauncherWeapon.h"
#include "Weapon/STUProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"


void ASTULauncherWeapon::StartFire()
{
    
	MakeShot();
  
}

void ASTULauncherWeapon::MakeShot()
{
    
    if (!GetWorld())
        return;
    if (IsAmmoEmpty())
    {
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), NoAmmoSound, GetActorLocation());
		return;
    }
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
  UGameplayStatics::SpawnSoundAttached(FireSound, WeaponMesh, MuzzleSocketName);
}