#pragma once

#include "Ability.h"
#include "CoreMinimal.h"
#include "WallAbility.generated.h"

UCLASS()
class TOPDOWNARPG_API UWallAbility : public UAbility {
  GENERATED_BODY()

 public:
  virtual void Activate(AActor* source) override;

 private:
  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<class AMeshProjectile> ProjectileClass;

  UPROPERTY(EditDefaultsOnly)
  TSubclassOf<class AStaticStructure> WallClass;

  AActor* Projectile;
};
