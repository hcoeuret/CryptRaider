// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber2.h"
#include "Engine/world.h"
#include "DrawDebugHelpers.h"


// Sets default values for this component's properties
UGrabber2::UGrabber2()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber2::BeginPlay()
{
	Super::BeginPlay();
	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Name of the component %s"), *PhysicsHandle->GetName());
	}
	else{
		UE_LOG(LogTemp, Warning, TEXT("No physics handle found !"));
	}
}


// Called every frame
void UGrabber2::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		return;
	}
	if(PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		FVector TargetLocation = GetComponentLocation() + GetForwardVector() * HoldDistance;
		PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
	}
}

void UGrabber2::Grab()
{
	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();
	if(PhysicsHandle == nullptr)
	{
		return;
	}

	FHitResult HitResult;
	bool HasHit = GetGrabbableInReach(HitResult);

	if(HasHit)
	{
		UPrimitiveComponent * HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		HitComponent->SetSimulatePhysics(true);

		HitResult.GetActor()->Tags.Add("Grabbed");
		HitResult.GetActor()->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		DrawDebugSphere(GetWorld(),HitResult.ImpactPoint,30,10,FColor::Blue,true);
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitComponent, 
			NAME_None, 
			HitResult.ImpactPoint,
			GetComponentRotation()
			);
	}
}

void UGrabber2::Release()
{
	UPhysicsHandleComponent * PhysicsHandle = GetPhysicsHandle();

	if(PhysicsHandle && PhysicsHandle->GetGrabbedComponent() != nullptr)
	{
		PhysicsHandle->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");
		PhysicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent * UGrabber2::GetPhysicsHandle() const
{
	UPhysicsHandleComponent * PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No physics"));
	}
	return PhysicsHandle; 
}

bool UGrabber2::GetGrabbableInReach(FHitResult& OutHitResult) const
{
	FVector Start = GetComponentLocation();
	FVector Forward = GetForwardVector();
	FVector End = Start + Forward * MaxGrabDistance ;
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	
	return GetWorld()->SweepSingleByChannel(
		OutHitResult, 
		Start, 
		End, 
		FQuat::Identity,
		ECC_GameTraceChannel3,
		Sphere
		);
}
