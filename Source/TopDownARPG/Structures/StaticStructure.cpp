#include "StaticStructure.h"
#include "TopDownARPG.h"

AStaticStructure::AStaticStructure()
{
    PrimaryActorTick.bCanEverTick = true;

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Structure"));
}

void AStaticStructure::BeginPlay()
{
    Super::BeginPlay();
}

void AStaticStructure::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
