// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemComponent.h"
#include "InventoryComponent.generated.h"

UENUM()
enum InventoryStatus
{
    ItemAdded          UMETA(DisplayName = "ItemAdded"),
    CantCarryMore      UMETA(DisplayName = "CantCarryMore"),
    NothingToPickUp    UMETA(DisplayName = "NothingToPickUp"),
};


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FInventoryChangedDelegate, UInventoryComponent, OnItemChanged, int, changedIdx);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FInventoryOpenChanged, UInventoryComponent, OnInventoryOpenChanged, bool, Open);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FOnItemAdded, UInventoryComponent, OnItemAdded, InventoryStatus, Status);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOPDOWNARPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent(); 
    
private:
    UFUNCTION()
    void BeginOverlap(AActor* mine, AActor* other);
    UFUNCTION()
    void EndOverlap(AActor* mine, AActor* other);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UFUNCTION(BlueprintCallable)
    void PickUpItem();

    UFUNCTION(BlueprintCallable)
    void AddItem(UItemComponent* item);

    UFUNCTION(BlueprintCallable)
    void DropItem(int slotIdx);

    UFUNCTION(BlueprintCallable)
    void MoveItem(int from, int to);

    UFUNCTION(BlueprintCallable)
    TArray<UItemComponent*> GetItems();

    UFUNCTION(BlueprintCallable)
    UItemComponent* ItemAt(int idx);

    UFUNCTION(BlueprintCallable)
    int Stacks(int idx);

    virtual void Activate(bool bReset = false) override;

public:

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    int ItemSlots;

    UPROPERTY(BlueprintAssignable)
    FInventoryChangedDelegate OnItemChanged;

    UPROPERTY(BlueprintAssignable)
    FInventoryOpenChanged OnInventoryOpenChanged;

    UPROPERTY(BlueprintAssignable)
    FOnItemAdded OnItemAdded;

    UPROPERTY(BlueprintReadOnly)
    bool IsOpen = false;

private:

    UPROPERTY()
    UItemComponent* OverlappedItem;

    UPROPERTY()
	TArray<UItemComponent*> Items;
};
