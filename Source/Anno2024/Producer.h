// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Producer.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ANNO2024_API UProducer : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UProducer();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(EditAnywhere) int storage = 6;
	UPROPERTY(EditAnywhere) TArray<int> resources;
	UPROPERTY(EditAnywhere) int produce = 0; // max 3

	float ticks = 0;
	UPROPERTY(EditAnywhere) float prodTime = 5.0f;
	UPROPERTY(EditAnywhere) bool producing = false;
	UPROPERTY(EditAnywhere) int type = 1;

	bool HasStorageSpace(int index);

private:
	void CheckProduction();
	void Produce();
	bool IsReadyToProduce();

public:
	float GetProductionProgress();
};

UENUM()
enum class ResourceType : uint8 {
	NEED_RESOURCES = 0,
	FROM_NATURE = 1,
	SEWING_MACHINE_FACTORY
};