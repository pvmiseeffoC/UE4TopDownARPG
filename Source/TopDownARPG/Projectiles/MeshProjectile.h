// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeshProjectile.generated.h"

UCLASS(Abstract, Blueprintable)
class AMeshProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMeshProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, Category = Visual)
	class UParticleSystemComponent* OnOverlapParticle;

	UPROPERTY(VisibleAnywhere, Category = Movement)
	class UProjectileMovementComponent* MovementComponent;

	UPROPERTY(EditAnywhere)
	float Damage;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
    DECLARE_DELEGATE_OneParam(FOnOverlapDelegate, AActor*);
    FOnOverlapDelegate OverlapDelegate;
};
