// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponents.h"

UTriggerComponents::UTriggerComponents()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UTriggerComponents::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UTriggerComponents::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (Mover == nullptr) return;
    AActor* AcceptableActor = GetAcceptableActor();
    if(AcceptableActor != nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Unlocking"));
        UPrimitiveComponent* Component = Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent());
        if(Component != nullptr)
        {
            Component->SetSimulatePhysics(false);
        }
        AcceptableActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Relocking"));
        Mover->SetShouldMove(false);
    }
    
}

AActor* UTriggerComponents::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for (AActor* Actor : Actors)
    {
        if(Actor->ActorHasTag("Unlock") && !Actor->ActorHasTag("Grabbed"))
        {
            return Actor;
        }
    } 
    return nullptr;  
}

void UTriggerComponents::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}