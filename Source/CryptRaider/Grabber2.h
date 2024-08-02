// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Grabber2.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CRYPTRAIDER_API UGrabber2 : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber2();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void Release();

	UFUNCTION(BlueprintCallable)
	void Grab();

	

private:
	UPROPERTY(EditAnywhere)
	float MaxGrabDistance = 400;	

	UPROPERTY(EditAnywhere)
	float GrabRadius = 10;

	UPROPERTY(EditAnywhere)
	float HoldDistance = 200;

	UPhysicsHandleComponent * GetPhysicsHandle() const;

	bool GetGrabbableInReach(FHitResult& OutHitResult) const;
};
