// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	owner = GetOwner();
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	float curTime = GetWorld()->GetTimeSeconds();

	if (!pressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing PressurePlate"), *owner->GetName());
	}
	else
	{
		if (GetTotalMassOfActorsOnPlate() > 40.f)
		{
			OnOpen.Broadcast();
		}
		else
		{
			OnClose.Broadcast();
		}
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.0f;
	TArray<AActor*> actors;
	pressurePlate->GetOverlappingActors(OUT actors);

	for (const auto& actor : actors)
	{
		totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return totalMass;
}

