// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "Engine/World.h"
#include "TopDownARPG/Characters/TopDownARPGCharacter.h"
#include <algorithm>

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UInventoryComponent::BeginOverlap(AActor * mine, AActor* other)
{
    if( auto itemComp = other->GetComponentByClass(UItemComponent::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("BEGIN OVERLAP WITH ITEM"));
        OverlappedItem = Cast<UItemComponent>(itemComp);
    }
}

void UInventoryComponent::EndOverlap(AActor * mine, AActor * other)
{
    if (auto itemComp = other->GetComponentByClass(UItemComponent::StaticClass()))
    {
        UE_LOG(LogTemp, Warning, TEXT("END OVERLAP WITH ITEM"));
        OverlappedItem = nullptr;
    }
}

// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

    auto owner = GetOwner();
    owner->OnActorBeginOverlap.AddDynamic(this, &UInventoryComponent::BeginOverlap);
    owner->OnActorEndOverlap.AddDynamic(this, &UInventoryComponent::EndOverlap);

    Items.Init(nullptr, ItemSlots);
	// ...
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::PickUpItem()
{
    if(OverlappedItem)
        AddItem(OverlappedItem);
    else
        OnItemAdded.Broadcast(InventoryStatus::NothingToPickUp);
}

void UInventoryComponent::AddItem(UItemComponent * item)
{
    auto itemCount = Items.Num();
    auto firstNull = -1;

    for(auto idx = 0; idx < itemCount && item->Stacks > 0; ++idx)
    {
        if(IsValid(Items[idx]))
        {
            if(Items[idx]->Name.IsEqual(item->Name) && Items[idx]->Stacks < Items[idx]->MaxStacks)
            {
                auto diff = std::min(Items[idx]->MaxStacks - Items[idx]->Stacks, item->Stacks);
                item->Stacks -= diff;
                Items[idx]->Stacks += diff;
                OnItemChanged.Broadcast(idx);
            }
        }
        else if (firstNull == -1)
        {
            firstNull = idx;
        }
    }

    if ( firstNull == -1 && item->Stacks > 0)
    {
        OnItemAdded.Broadcast(InventoryStatus::CantCarryMore);
        return;
    }
    else if (firstNull != -1 && item->Stacks > 0)
    {
        Items[firstNull] = item;
        OnItemChanged.Broadcast(firstNull);
    }

    auto itemOwner = item->GetOwner();
    itemOwner->SetActorHiddenInGame(true);
    itemOwner->SetActorEnableCollision(false);
    OnItemAdded.Broadcast(InventoryStatus::ItemAdded);
}

void UInventoryComponent::DropItem(int slotIdx)
{
    auto owner = GetOwner();
    auto dropLocation = owner->GetActorLocation() + owner->GetActorForwardVector() * 100.0f;

    auto dropOwner = Items[slotIdx]->GetOwner();
    dropOwner->SetActorLocation(dropLocation);
    dropOwner->SetActorHiddenInGame(false);
    dropOwner->SetActorEnableCollision(true);
    Items[slotIdx] = nullptr;
    OnItemChanged.Broadcast(slotIdx);
}

void UInventoryComponent::MoveItem(int from, int to)
{
    std::swap(Items[from], Items[to]);
    OnItemChanged.Broadcast(from);
    OnItemChanged.Broadcast(to);
}

TArray<UItemComponent*> UInventoryComponent::GetItems()
{
    return Items;
}

UItemComponent * UInventoryComponent::ItemAt(int idx)
{
    return Items[idx];
}

int UInventoryComponent::Stacks(int idx)
{
    return Items[idx]->Stacks;
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
