// Fill out your copyright notice in the Description page of Project Settings.


#include "OverseerPawn.h"
#include "Producer.h"
#include "TruckMovement.h"
#include "ProductionInterfaceWidget.h"
#include "Blueprint/UserWidget.h"

void AOverseerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (GameWidgetClass) {
		GameWidget = Cast<UProductionInterfaceWidget>(CreateWidget(GetWorld(), GameWidgetClass));

		if (GameWidget) {
			GameWidget->AddToViewport();
			this->UpdateText();
		}
	}

	this->SpawnTrucks();
}

void AOverseerPawn::SpawnTrucks()
{
	for (AActor* truck : this->trucks) {
		if (truck && truck->GetActorLocation().Z != 0) {
			float randX = FMath::RandRange(-5350, 5350);

			while ((randX >= -5000 && randX <= -3450) ||
				   (randX <= 5000 && randX >= 3450))
				randX = FMath::RandRange(-5350, 5350);

			int randDir = FMath::RandRange(1, 2);
			float randY = FMath::RandRange(-50, 200);

			if (randDir == 1) 
				randY = -50;
			else 
				randY = 200;

			float yaw = 0;

			if (randX < -4900 || randX > 4900) { // If on the side roads,
				randY = FMath::RandRange(-4350, 4350);
				if (randX < -4900) { // first st
					if (randX < -5200) yaw = 90;
					else yaw = -90;
				}
				else {
					if (randX > 5200) yaw = -90;
					else yaw = 90;
				}
			}
			else {
				if (randY < 100)  // first st
					yaw = 180;
				else 
					yaw = 0;
			}
			
			FVector spawnLoc = FVector(randX, randY, 0);
			truck->SetActorLocation(spawnLoc);
			FRotator truckTurn = truck->GetActorRotation();
			truckTurn.Yaw = yaw;
			truck->SetActorRotation(truckTurn);
		}
	}
}

void AOverseerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	this->UpdateText();

	this->timer += DeltaTime;
	if (this->timer >= 60.0f) {
		UE_LOG(LogTemp, Display, TEXT("Produced %d Sewing Machines."), buildings[4]->FindComponentByClass<UProducer>()->produce);
	}
}

void AOverseerPawn::UpdateText()
{
	if (buildings[0]) {
		GameWidget->SetIronMineStockText(buildings[0]->FindComponentByClass<UProducer>()->produce);
		GameWidget->SetIronMineProductionBar(buildings[0]->FindComponentByClass<UProducer>()->GetProductionProgress(),
											buildings[0]->FindComponentByClass<UProducer>()->ticks,
											buildings[0]->FindComponentByClass<UProducer>()->prodTime);
	}
	if (buildings[1]) {
		GameWidget->SetCoalMineStockText(buildings[1]->FindComponentByClass<UProducer>()->produce);
		GameWidget->SetCoalMineProductionBar(buildings[1]->FindComponentByClass<UProducer>()->GetProductionProgress(),
											buildings[1]->FindComponentByClass<UProducer>()->ticks,
											buildings[1]->FindComponentByClass<UProducer>()->prodTime);
	}
	if (buildings[2]) {
		GameWidget->SetFurnaceStockText(buildings[2]->FindComponentByClass<UProducer>()->produce);
		GameWidget->SetIronStorageText(buildings[2]->FindComponentByClass<UProducer>()->resources[0]);
		GameWidget->SetCoalStorageText(buildings[2]->FindComponentByClass<UProducer>()->resources[1]);
		GameWidget->SetFurnaceProductionBar(buildings[2]->FindComponentByClass<UProducer>()->GetProductionProgress(),
											buildings[2]->FindComponentByClass<UProducer>()->ticks,
											buildings[2]->FindComponentByClass<UProducer>()->prodTime);
	}
	if (buildings[3]) {
		GameWidget->SetLumberHutStockText(buildings[3]->FindComponentByClass<UProducer>()->produce);
		GameWidget->SetLumberHutProductionBar(buildings[3]->FindComponentByClass<UProducer>()->GetProductionProgress(),
											buildings[3]->FindComponentByClass<UProducer>()->ticks,
											buildings[3]->FindComponentByClass<UProducer>()->prodTime);
	}
	if (buildings[4]) {
		GameWidget->SetSewingMachineStockText(buildings[4]->FindComponentByClass<UProducer>()->produce);
		GameWidget->SetSteelStorageText(buildings[4]->FindComponentByClass<UProducer>()->resources[0]);
		GameWidget->SetLumberStorageText(buildings[4]->FindComponentByClass<UProducer>()->resources[1]);
		GameWidget->SetSewingMachineProductionBar(buildings[4]->FindComponentByClass<UProducer>()->GetProductionProgress(),
												buildings[4]->FindComponentByClass<UProducer>()->ticks,
												buildings[4]->FindComponentByClass<UProducer>()->prodTime);
	}

	if (trucks[0]) {
		GameWidget->SetTruckAState(FName(*this->GetTruckState(0)));
		GameWidget->SetTruckAIronCount(trucks[0]->FindComponentByClass<UTruckMovement>()->resourcesHeld[0]);
		GameWidget->SetTruckACoalCount(trucks[0]->FindComponentByClass<UTruckMovement>()->resourcesHeld[1]);
		GameWidget->SetTruckASteelCount(trucks[0]->FindComponentByClass<UTruckMovement>()->resourcesHeld[2]);
		GameWidget->SetTruckALumberCount(trucks[0]->FindComponentByClass<UTruckMovement>()->resourcesHeld[3]);
		GameWidget->SetTruckALoadingTime(trucks[0]->FindComponentByClass<UTruckMovement>()->loadingTime);
		GameWidget->SetTruckAStorage(trucks[0]->FindComponentByClass<UTruckMovement>()->maxResources);
	}
	if (trucks[1]) {
		GameWidget->SetTruckBState(FName(*this->GetTruckState(1)));
		GameWidget->SetTruckBIronCount(trucks[1]->FindComponentByClass<UTruckMovement>()->resourcesHeld[0]);
		GameWidget->SetTruckBCoalCount(trucks[1]->FindComponentByClass<UTruckMovement>()->resourcesHeld[1]);
		GameWidget->SetTruckBSteelCount(trucks[1]->FindComponentByClass<UTruckMovement>()->resourcesHeld[2]);
		GameWidget->SetTruckBLumberCount(trucks[1]->FindComponentByClass<UTruckMovement>()->resourcesHeld[3]);
		GameWidget->SetTruckBLoadingTime(trucks[1]->FindComponentByClass<UTruckMovement>()->loadingTime);
		GameWidget->SetTruckBStorage(trucks[1]->FindComponentByClass<UTruckMovement>()->maxResources);
	}
}

FString AOverseerPawn::GetTruckState(int index)
{
	FString state = "";

	if (trucks[index]) {
		switch (trucks[index]->FindComponentByClass<UTruckMovement>()->state) {
			case TruckState::MOVING:
			case TruckState::TURNING:
				state = "In Transit";
				break;
			case TruckState::LEAVING:
			case TruckState::DOCKED:
				state = "Parked";
				break;
			case TruckState::IDLE:
				state = "On Standby";
				break;
		}
	}

	return state;
}

