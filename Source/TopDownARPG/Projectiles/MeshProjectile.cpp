// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshProjectile.h"
#include "TopDownARPGCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TopDownARPG.h"

// Sets default values
AMeshProjectile::AMeshProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
    MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
    MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
    MeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	RootComponent = MeshComponent;

	MeshComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AMeshProjectile::OnOverlap);

	OnOverlapParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	OnOverlapParticle->bAutoActivate = false;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
    MovementComponent->Velocity.X = 1.0;
    MovementComponent->Velocity.Y = 0.0;
    MovementComponent->Velocity.Z = 0.0;
}

// Called when the game starts or when spawned
void AMeshProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AMeshProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}
    if (OverlapDelegate.IsBound())
        OverlapDelegate.Execute(Other);
    else
    {
        ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
        if (IsValid(Character))
        {
            Character->TakeDamage(Damage, FDamageEvent(UDamageType::StaticClass()), nullptr, this);
        }

        if (Destroy() == false)
        {
            UE_LOG(LogTopDownARPG, Error, TEXT("Projectile is indestructable"));
        }
    }
}

// Called every frame
void AMeshProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

