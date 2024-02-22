// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProductionInterfaceWidget.generated.h"

/**
 * 
 */
UCLASS()
class ANNO2024_API UProductionInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	// Stocks
	UFUNCTION(BlueprintImplementableEvent) void SetIronMineStockText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetCoalMineStockText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetFurnaceStockText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetLumberHutStockText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetSewingMachineStockText(int32 stock);

	// Stocks
	UFUNCTION(BlueprintImplementableEvent) void SetIronStorageText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetCoalStorageText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetSteelStorageText(int32 stock);
	UFUNCTION(BlueprintImplementableEvent) void SetLumberStorageText(int32 stock);

	// Production
	UFUNCTION(BlueprintImplementableEvent) void SetIronMineProductionBar(float progress, float ticks, float prodTime);
	UFUNCTION(BlueprintImplementableEvent) void SetCoalMineProductionBar(float progress, float ticks, float prodTime);
	UFUNCTION(BlueprintImplementableEvent) void SetFurnaceProductionBar(float progress, float ticks, float prodTime);
	UFUNCTION(BlueprintImplementableEvent) void SetLumberHutProductionBar(float progress, float ticks, float prodTime);
	UFUNCTION(BlueprintImplementableEvent) void SetSewingMachineProductionBar(float progress, float ticks, float prodTime);

	// Truck A States
	UFUNCTION(BlueprintImplementableEvent) void SetTruckAState(FName State);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckAIronCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckACoalCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckASteelCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckALumberCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckALoadingTime(int time);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckAStorage(int storage);

	// Truck B States
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBState(FName State);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBIronCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBCoalCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBSteelCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBLumberCount(int count);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBLoadingTime(int time);
	UFUNCTION(BlueprintImplementableEvent) void SetTruckBStorage(int storage);
};
