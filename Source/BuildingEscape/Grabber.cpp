// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "Grabber.h"


// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();
	FindInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!physicHandle) return;
	// if Grabbed component move it
	if (physicHandle->GrabbedComponent)
	{
		physicHandle->SetTargetLocation(GetReachLineEnd());
	}
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{

	physicHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physicHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

// Look for attached Input Component (Only appears at run time)
void UGrabber::FindInputComponent()
{

	inputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (inputComponent)
	{
		inputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::GrabPressed);
		inputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

void UGrabber::GrabPressed()
{
	auto hit = GetFirstPhysicsBodyInReach();
	auto componentToGrab = hit.GetComponent();
	auto actorHit = hit.GetActor();

	if (actorHit)
	{
		if (!physicHandle) return;
		physicHandle->GrabComponent(
			componentToGrab,
			NAME_None, // no bones needed
			componentToGrab->GetOwner()->GetActorLocation(),
			true// allow rotation
			);
	}
}

void UGrabber::GrabReleased()
{
	if (!physicHandle) return;
	physicHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FCollisionQueryParams traceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECC_PhysicsBody),
		traceParameters
		);

	return hit;
}

FVector UGrabber::GetReachLineStart()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation, OUT playerViewPointRotation
		);

	return playerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	FVector playerViewPointLocation;
	FRotator playerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT playerViewPointLocation, OUT playerViewPointRotation
		);

	return playerViewPointLocation + playerViewPointRotation.Vector()*reach;
}

