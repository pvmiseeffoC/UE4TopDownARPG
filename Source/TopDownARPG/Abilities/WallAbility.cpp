#include "WallAbility.h"
#include "Projectiles/MeshProjectile.h"
#include "Structures/StaticStructure.h"
#include "TopDownARPG.h"
#include "Engine/World.h"
#include "EngineGlobals.h"
#include <Runtime/Engine/Classes/Engine/Engine.h>
#include <Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h>

void UWallAbility::Activate(AActor* Source)
{
    UWorld* World = GetWorld();
    if (IsValid(World) == false)
    {
        UE_LOG(LogTopDownARPG, Error, TEXT("UWallAbility::Activate IsValid(World) == false"));
    }

    if (!Projectile || !IsValid(Projectile))
    {
        FActorSpawnParameters SpawnParameters;
        SpawnParameters.Owner = Source;
        SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

        FVector SpawnLocation = Source->GetActorLocation() + Source->GetActorForwardVector() * 100.0f;

        Projectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, Source->GetActorRotation(), SpawnParameters);

        if (IsValid(Projectile) == false)
        {
            UE_LOG(LogTopDownARPG, Error, TEXT("UWallAbility::Activate IsValid(Projectile) == false"));
            return;
        }
        else
        {
            auto derived = Cast<AMeshProjectile>(Projectile);
            derived->OverlapDelegate.BindLambda([this, Source](AActor* other)
                {
                    if(Source != other)
                        Activate(Source);
                });
        }
    }
    else
    {
        auto WallLocation = Projectile->GetActorLocation();
        Projectile->Destroy();
        Projectile = nullptr;
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        
        auto ActorLocation = Source->GetActorLocation();
        auto Perpendicular = FVector::CrossProduct(ActorLocation, WallLocation);
        auto PerpendicularRotation = Perpendicular.Rotation();
        PerpendicularRotation.Pitch = 0;
        PerpendicularRotation.Roll = 0;
        
        auto Wall = World->SpawnActor<AActor>(WallClass, WallLocation, PerpendicularRotation, SpawnParams);
        if (IsValid(Wall) == false)
        {
            UE_LOG(LogTopDownARPG, Error, TEXT("UWallAbility::Activate IsValid(Wall) == false"));
            return;
        }

        Super::Activate(Source);
    }
}