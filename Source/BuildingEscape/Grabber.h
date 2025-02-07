// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	// How far ahead of the player can we reach in cm
	float reach = 100.f;

	UPhysicsHandleComponent* physicHandle = nullptr;

	UInputComponent* inputComponent = nullptr;

	void FindPhysicsHandleComponent();
	void FindInputComponent();
	void GrabPressed();
	void GrabReleased();
	const FHitResult GetFirstPhysicsBodyInReach();
	FVector GetReachLineStart();
	FVector GetReachLineEnd();
};
