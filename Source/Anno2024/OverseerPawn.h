// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "OverseerPawn.generated.h"

class UProducer;
class UProductionInterfaceWidget;
/**
 * 
 */
UCLASS()
class ANNO2024_API AOverseerPawn : public APawn
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Widgets")
	TSubclassOf<class UUserWidget> GameWidgetClass;

	UPROPERTY()
	UProductionInterfaceWidget* GameWidget;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnTrucks();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere) TArray<AActor*> trucks;
	UPROPERTY(EditAnywhere) TArray<AActor*> buildings;

	UFUNCTION(BlueprintCallable) void UpdateText();

	float timer = 0;
	FString GetTruckState(int index);
};
