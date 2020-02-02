// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/Texture2D.h"
#include "ItemComponent.generated.h"

UENUM()
enum class ItemClass {
    Normal UMETA(DisplayName = "Normal"),
    Rare UMETA(DisplayNAme = "Rare"),
    Legendary UMETA(DisplayName = "Legendary"),
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWNARPG_API UItemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TAssetPtr<UTexture2D> Icon;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName Name;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName Description;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int MaxStacks;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool Consumable;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    ItemClass Rarity;
};
