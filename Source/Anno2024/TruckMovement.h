// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerVolume.h"
#include "Components/ActorComponent.h"
#include "TruckMovement.generated.h"

UENUM()
enum class BuildingType : uint8 {
	LOAD_ONLY = 0,
	LOAD_UNLOAD = 1,
	UNLOAD_ONLY = 2,
	NONE
};

UENUM()
enum class TruckState : uint8 {
	MOVING = 0,
	TURNING = 1,
	DOCKED = 2,
	LEAVING = 3,
	IDLE
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNO2024_API UTruckMovement : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTruckMovement();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere) TruckState state = TruckState::IDLE;
	UPROPERTY(EditAnywhere) bool rightTurner = true;

	UPROPERTY(EditAnywhere) TArray<int32> resourcesHeld;
	UPROPERTY(EditAnywhere) int32 loadingTime = 1;
	UPROPERTY(EditAnywhere) int32 maxResources = 2;

private:
	UPROPERTY(EditAnywhere) float speed = 21.0f;
	UPROPERTY(EditAnywhere) float step = 100.0f;
	UPROPERTY(EditAnywhere) float travelTime =	4.0f;

	UPROPERTY(EditAnywhere) float rightTurn = 90.0f;
	UPROPERTY(EditAnywhere) float leftTurn = -90.0f;
	float flip = 180.0f;
	UPROPERTY(EditAnywhere) TArray<ATriggerVolume*> intersections;
	UPROPERTY(EditAnywhere) TArray<ATriggerVolume*> buildings;
	UPROPERTY(EditAnywhere) TArray<AActor*> roads;
	UPROPERTY(EditAnywhere) AActor* truckTurner;

	float ticks = 0.0f;
	float angleTicks = 0.0f;
	UPROPERTY(EditAnywhere) bool turn = false;
	UPROPERTY(EditAnywhere) bool docked = false;
	UPROPERTY(EditAnywhere) bool done = false;

	float initialYaw;
	float currentYaw;

	int32 resourcesStored = 0;
	UPROPERTY(EditAnywhere) int lastVisited = -1;

	void CheckMove(float deltaTime);
	void Recenter(float deltaTime);
	void MoveForward(float deltaTime);
	void TurnRight(float deltaTime);
	void TurnLeft(float deltaTime);
	void FlipAround();

	void CheckBuilding(int buildingIndex, float deltaTime);
	void LoadResource(int resource);
	void UnloadResource(int resource, int index);
	
	bool IsAtIntersection();
	int IsAtBuilding();
	bool IsReadyToTransport();
	int TotalResourcesHeld();
};