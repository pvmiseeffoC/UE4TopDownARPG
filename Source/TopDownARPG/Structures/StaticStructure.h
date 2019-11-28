#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StaticStructure.generated.h"

UCLASS()
class TOPDOWNARPG_API AStaticStructure : public AActor {
  GENERATED_BODY()

 public:
  // Sets default values for this actor's properties
  AStaticStructure();

 protected:
  // Called when the game starts or when spawned
  virtual void BeginPlay() override;

 public:
  // Called every frame
  virtual void Tick(float DeltaTime) override;

  UPROPERTY(VisibleAnywhere)
  UStaticMeshComponent* StaticMesh;
};
