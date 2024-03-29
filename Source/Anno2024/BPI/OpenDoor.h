// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNO2024_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere) float openingYaw = 90.0f;
	UPROPERTY(EditAnywhere) float closingYaw = 0.0f;
	UPROPERTY(EditAnywhere) ATriggerVolume* pressurePlate;
	//UPROPERTY(EditAnywhere) AActor* actorOpener;

	float ticks = 0.0f;
	float initialYaw;
	float currentYaw;
	float totalMass = 180.0f;

	enum DoorState { OPEN = 0, CLOSED = 1};
	DoorState doorState = OPEN;

	float GetPressurePlateTotalMass() const;
};
