// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Engine/World.h"
#include "TopDownARPG/Characters/TopDownARPGCharacter.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UInventoryComponent::BeginOverlap(AActor * mine, AActor* other)
{
    if( auto itemComp = other->GetComponentByClass(UItemComponent::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("YAAAAAAAAY I OVERLAPPED AN ITEM"));
        other->SetActorHiddenInGame(true);
        other->SetActorEnableCollision(false);
        AddItem(Cast<UItemComponent>(itemComp));
    }
}

void UInventoryComponent::EndOverlap(AActor * mine, AActor * other)
{
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    auto owner = GetOwner();
    owner->OnActorBeginOverlap.AddDynamic(this, &UInventoryComponent::BeginOverlap);

    items.Init(nullptr, ItemSlots);
    StacksPerSlot.Init(0, ItemSlots);
	// ...
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

ItemStatus UInventoryComponent::AddItem(UItemComponent * item)
{
    auto itemCount = items.Num();
    auto firstNull = -1;
    for(auto idx = 0; idx < itemCount; ++idx)
    {
        if(IsValid(items[idx]))
        {
            if(items[idx]->Name.IsEqual(item->Name) && StacksPerSlot[idx] < items[idx]->MaxStacks)
            {
                StacksPerSlot[idx] += 1;
                OnItemChanged.Broadcast(idx);
                return ItemStatus::ItemAdded;
            }
        }
        else if (firstNull == -1)
        {
            firstNull = idx;
        }
    }

    if(firstNull != -1)
    {
        StacksPerSlot[firstNull] = 1;
        items[firstNull] = item;
        OnItemChanged.Broadcast(firstNull);
        return ItemStatus::ItemAdded;;
    }
    
    return ItemStatus::OutOfSpace;
}

TArray<UItemComponent*> UInventoryComponent::GetItems()
{
    return items;
}

UItemComponent * UInventoryComponent::ItemAt(int idx)
{
    return items[idx];
}

int UInventoryComponent::Stacks(int idx)
{
    return StacksPerSlot[idx];
}

void UInventoryComponent::Activate(bool bReset)
{
    if(IsOpen)
    {
        Super::Deactivate();
    }
    else
    {
        Super::Activate(bReset);
    }
    IsOpen = !IsOpen;
    OnInventoryOpenChanged.Broadcast(IsOpen);
}
