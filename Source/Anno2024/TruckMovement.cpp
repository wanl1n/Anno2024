// Fill out your copyright notice in the Description page of Project Settings.


#include "TruckMovement.h"
#include "Producer.h"

class UProducer;

// Sets default values for this component's properties
UTruckMovement::UTruckMovement()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTruckMovement::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->resourcesHeld.Init(0, 4);

	this->initialYaw = this->GetOwner()->GetActorRotation().Yaw;
	this->currentYaw = this->initialYaw;
	this->rightTurn += this->initialYaw;
	this->leftTurn += this->initialYaw;
}


// Called every frame
void UTruckMovement::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	/*UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Turning %s."), *this->GetOwner()->GetName(), this->turn ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Docked %s."), *this->GetOwner()->GetName(), this->docked ? TEXT("true") : TEXT("false"));
	UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Done %s."), *this->GetOwner()->GetName(), this->done ? TEXT("true") : TEXT("false"));*/

	//FVector currentPos = this->GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Tick Current position: (%f, %f) at %f"), *this->GetOwner()->GetName(), currentPos.X, currentPos.Y, this->ticks);

	this->ticks += DeltaTime;
	this->angleTicks += DeltaTime * speed;
	//if (this->angleTicks >= 90) this->angleTicks = 0;

	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Ticks %f"), *this->GetOwner()->GetName(), this->ticks);
	if (this->docked && this->ticks >= this->loadingTime) {
		this->ticks = 0;

		if (this->docked) {
			this->docked = false;
			this->done = true;
			this->state = TruckState::LEAVING;

			if (this->IsAtBuilding() != 4)
				this->FlipAround(); 
		}
	}

	if (this->IsAtIntersection() && !this->turn) {
		if (!this->turn) {
			this->currentYaw = this->GetOwner()->GetActorRotation().Yaw;
			//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Current Yaw. %f"), *this->GetOwner()->GetName(), this->currentYaw);
			this->rightTurn = this->currentYaw + 90;
			this->leftTurn = this->currentYaw - 90;
			this->angleTicks = 0;
		}
		this->turn = true;
	}

	if (this->IsReadyToTransport())
		this->CheckMove(DeltaTime);
}

void UTruckMovement::CheckMove(float deltaTime)
{
	FVector currentPos = this->GetOwner()->GetActorLocation();
	if (this->IsAtBuilding() != -1 && !this->done) {
		if (!this->docked) {
			this->loadingTime = FMath::RandRange(1, 3);
			this->ticks = 0;
		}

		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Is At Building. %d"), *this->GetOwner()->GetName(), this->IsAtBuilding());
		this->state = TruckState::DOCKED;
		this->docked = true;
		this->CheckBuilding(this->IsAtBuilding(), deltaTime);
	}
	else if (this->turn) {
		this->state = TruckState::TURNING;
		if (this->rightTurner)
			this->TurnRight(deltaTime);
		else
			this->TurnLeft(deltaTime);
	}
	else {
		if (this->IsAtBuilding() == -1)
			this->done = false;

		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Current Yaw. %f"), *this->GetOwner()->GetName(), this->currentYaw);
		this->currentYaw = this->GetOwner()->GetActorRotation().Yaw;
		this->state = TruckState::MOVING;
		this->MoveForward(deltaTime);
	}
}

void UTruckMovement::Recenter(float deltaTime)
{
	FVector currentPos = this->GetOwner()->GetActorLocation();
	FVector truckOffcenter = currentPos;

	// If truck is on Central Avenue:
	if (currentPos.X <= 50.0f * 100 && currentPos.X >= -49.0f * 100) {
		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Currently at Central Avenue from %f. "), *this->GetOwner()->GetName(), currentYaw);

		switch (lastVisited) {
			case 0:
			case 1:
				//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : From First Street. %f. "), *this->GetOwner()->GetName(), currentYaw);
				truckOffcenter = FVector(currentPos.X, 2.5f * 100, currentPos.Z);
				break;
			case 2:
			case 3:
				//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : From Second Street. %f. "), *this->GetOwner()->GetName(), currentYaw);
				truckOffcenter = FVector(currentPos.X, -0.5f * 100, currentPos.Z);
				break;
			default:
				//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : From Central. %f. "), *this->GetOwner()->GetName(), currentYaw);
				truckOffcenter = currentPos;
				break;
		}

		if (this->currentYaw == 0 && (this->lastVisited == 0 || this->lastVisited == 1)) {
			truckOffcenter = FVector(currentPos.X, 2.5f * 100, currentPos.Z);
		}
	}
	// If truck is either on First or Second St.
	else {
		//UE_LOG(LogTemp, Display, TEXT("Truck Movement : On the Side Roads."));
		// If truck is on First Street
		if (currentPos.X <= -50.4f * 100) {
			//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Currently at First St from %f. "), *this->GetOwner()->GetName(), currentYaw);

			if (this->currentYaw < 0 || this->currentYaw > 180)
				truckOffcenter = FVector(-50.5f * 100, currentPos.Y, currentPos.Z);
			else if (this->currentYaw > 0)
				truckOffcenter = FVector(-53.5f * 100, currentPos.Y, currentPos.Z);
		}
		// If truck is on Second Street
		else if (currentPos.X >= 50.4f * 100) {
			//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Currently at Second St from %f."), *this->GetOwner()->GetName(), currentYaw);
			if (this->currentYaw < 0 || this->currentYaw > 180)
				truckOffcenter = FVector(53.5f * 100, currentPos.Y, currentPos.Z);
			else if(this->currentYaw > 0)
				truckOffcenter = FVector(50.5f * 100, currentPos.Y, currentPos.Z);
		}
		else {
			//UE_LOG(LogTemp, Display, TEXT("Truck Movement : Centered."));
			truckOffcenter = currentPos;
		}
	}

	truckOffcenter = FMath::Lerp(currentPos, truckOffcenter, 1.0f);

	this->GetOwner()->SetActorLocation(truckOffcenter);
}

void UTruckMovement::MoveForward(float deltaTime)
{

	FVector currentPos = this->GetOwner()->GetActorLocation();
	FVector truckForward = this->GetOwner()->GetActorForwardVector() * this->step;
	truckForward += currentPos;

	if (this->turn)
		truckForward = FMath::Lerp(currentPos, truckForward, deltaTime * this->speed);
	else 
		truckForward = FMath::Lerp(currentPos, truckForward, this->ticks/this->travelTime);

	this->GetOwner()->SetActorLocation(truckForward);

	this->Recenter(deltaTime);
}

void UTruckMovement::TurnRight(float deltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement : Right Turn. %f"), this->rightTurn);
	this->currentYaw = FMath::Lerp(this->currentYaw, this->rightTurn, this->angleTicks / 90.0f);

	FRotator truckTurn = this->GetOwner()->GetActorRotation();
	truckTurn.Yaw = this->currentYaw;
	this->GetOwner()->SetActorRotation(truckTurn);
	this->MoveForward(deltaTime);

	if (this->rightTurn - this->currentYaw <= 1.0f) {
		this->turn = false;
		this->currentYaw = this->rightTurn;

		FRotator truckTurnEnd = this->GetOwner()->GetActorRotation();
		truckTurnEnd.Yaw = this->currentYaw;
		this->GetOwner()->SetActorRotation(truckTurnEnd);
	}
}

void UTruckMovement::TurnLeft(float deltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Left Turn. %f"), *this->GetOwner()->GetName(), this->currentYaw);
	this->currentYaw = FMath::Lerp(this->currentYaw, this->leftTurn, this->angleTicks / 90.0f);

	FRotator truckTurn = this->GetOwner()->GetActorRotation();
	truckTurn.Yaw = this->currentYaw;
	this->GetOwner()->SetActorRotation(truckTurn);
	FVector currentPos = this->GetOwner()->GetActorLocation();
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Left Turn Current position: (%f, %f) at %f"), *this->GetOwner()->GetName(), currentPos.X, currentPos.Y, this->ticks);


	this->MoveForward(deltaTime);

	if (this->currentYaw - this->leftTurn <= 1.0f) {
		this->turn = false;
		this->currentYaw = this->leftTurn;

		FRotator truckTurnEnd = this->GetOwner()->GetActorRotation();
		truckTurnEnd.Yaw = this->currentYaw;
		this->GetOwner()->SetActorRotation(truckTurnEnd);
	}
}

void UTruckMovement::FlipAround()
{
	// Turn 180 degrees.
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Current Yaw. %f"), *this->GetOwner()->GetName(), this->currentYaw);
	FRotator truckTurn = this->GetOwner()->GetActorRotation();
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Flip Truck Turn target. %f + %f"), *this->GetOwner()->GetName(), truckTurn.Yaw, this->flip);
	truckTurn.Yaw += this->flip;
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Flip. %f"), *this->GetOwner()->GetName(), truckTurn.Yaw);
	this->GetOwner()->SetActorRotation(truckTurn);
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Flip. %f"), *this->GetOwner()->GetName(), this->GetOwner()->GetActorRotation().Yaw);
	this->currentYaw = this->GetOwner()->GetActorRotation().Yaw;
}

void UTruckMovement::CheckBuilding(int buildingIndex, float deltaTime)
{
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement : Checking Building."));
	BuildingType type = BuildingType::NONE;

	switch (buildingIndex) {
		case 0: // Iron Mine
		case 1: // Coal Mine
		case 3: // Lumber Hut
			type = BuildingType::LOAD_ONLY;
			break;

		case 2: // Furnace
			type = BuildingType::LOAD_UNLOAD;
			break;

		case 4: // Sewing Machine Factory
			type = BuildingType::UNLOAD_ONLY;
			break;
	}

	UProducer* producer = this->buildings[buildingIndex]->FindComponentByClass<UProducer>();

	if (type == BuildingType::UNLOAD_ONLY) { // Sewing Machine Factory
		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Building for Unload Only. Storage: %d"), *this->GetOwner()->GetName(), buildingStorage);
		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Unloading Resource in Check Building."), *this->GetOwner()->GetName());
			
		// Steel Unloading
		if (producer->HasStorageSpace(0) &&
			this->resourcesHeld[2] > 0) {
			this->done = false;
			this->UnloadResource(2, 0);
		}
		// Lumber Unloading
		if (producer->HasStorageSpace(1) &&
			this->resourcesHeld[3] > 0) {
			this->done = false;
			this->UnloadResource(3, 1);
		}

		// If nothing to do here.
		if (this->resourcesHeld[2] <= 0 && this->resourcesHeld[3] <= 0 && this->ticks == 0) {
			this->ticks = 0;
			this->docked = false;
			this->done = true;
			this->state = TruckState::LEAVING;
		}
	}
	else if (type == BuildingType::LOAD_ONLY) {
		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Building for Load Only. Produce: %d"), *this->GetOwner()->GetName(), produceCount);

		// If nothing to do here.
		if ((this->TotalResourcesHeld() >= this->maxResources || producer->produce == 0) && 
			this->ticks == 0) {

			this->ticks = 0;
			this->docked = false;
			this->done = true;
			this->state = TruckState::LEAVING;
			this->FlipAround();
		}

		if (this->TotalResourcesHeld() < this->maxResources &&
			producer->produce > 0) {

			//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Load Resources Go."), *this->GetOwner()->GetName());
			this->done = false;
			this->LoadResource(buildingIndex);
		}
	}
	else if (type == BuildingType::LOAD_UNLOAD) { // Furnace
		//UE_LOG(LogTemp, Display, TEXT("Truck Movement %s : Building for Load and Unload. Storage: %d, Produce: %d"), *this->GetOwner()->GetName(), buildingStorage, produceCount);

		// If nothing to do here.
		if (this->resourcesHeld[0] <= 0 && this->resourcesHeld[1] <= 0 &&
			(this->TotalResourcesHeld() >= this->maxResources || producer->produce == 0) &&
			this->ticks == 0) {

			this->ticks = 0;
			this->docked = false;
			this->done = true;
			this->state = TruckState::LEAVING;
			this->FlipAround();
		}

		// Iron Unloading
		if (producer->HasStorageSpace(0) &&
			this->resourcesHeld[0] > 0) {
			this->done = false;
			this->UnloadResource(0, 0);
		}
		// Coal Unloading
		if (producer->HasStorageSpace(1) &&
			this->resourcesHeld[1] > 0) {
			this->done = false;
			this->UnloadResource(1, 1);
		}
		// Getting Steel from Furnace
		else if (producer->produce > 0 && this->TotalResourcesHeld() < this->maxResources)
			this->LoadResource(buildingIndex);
	}
}

void UTruckMovement::LoadResource(int resource)
{
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement : Loading a Resource."));
	this->resourcesHeld[resource]++;
	this->buildings[resource]->FindComponentByClass<UProducer>()->produce--;
}

void UTruckMovement::UnloadResource(int resource, int index)
{
	//UE_LOG(LogTemp, Display, TEXT("Truck Movement : Unloading a Resource."));
	this->resourcesHeld[resource]--;
	this->buildings[this->IsAtBuilding()]->FindComponentByClass<UProducer>()->storage--;
	this->buildings[this->IsAtBuilding()]->FindComponentByClass<UProducer>()->resources[index]++;
}

bool UTruckMovement::IsAtIntersection()
{
	int maxIndex = this->intersections.Num();

	if (this->rightTurner) {
		for (int i = 0; i < maxIndex - 2; i++) {
			if (this->truckTurner != NULL &&
				this->intersections[i]->IsOverlappingActor(this->truckTurner)) {
					return true;
			}
		}
	}
	else {
		for (int i = 0; i < maxIndex; i++) {
			if (this->truckTurner != NULL &&
				this->intersections[i]->IsOverlappingActor(this->truckTurner) &&
				i != 2 &&
				i != 3) {
				return true;
			}
		}
	}

	return false;
}

int UTruckMovement::IsAtBuilding()
{
	for (int i = 0; i < this->buildings.Num(); i++) {
		if (this->truckTurner != NULL &&
			this->buildings[i]->IsOverlappingActor(this->truckTurner)) {
			if (i != 4)
				this->lastVisited = i;
			return i;
		}
	}

	return -1;
}

bool UTruckMovement::IsReadyToTransport()
{
	bool transport = false;

	if (this->state == TruckState::IDLE) 
		this->ticks = 0;

	for (int i = 0; i < this->buildings.Num(); i++) {
		if (this->buildings[i]->FindComponentByClass<UProducer>()->produce > 0)
			transport = true;
	}

	return transport;
}

int UTruckMovement::TotalResourcesHeld()
{
	int total = 0;
	for (int i = 0; i < this->resourcesHeld.Num(); i++) {
		total += this->resourcesHeld[i];
	}
	return total;
}
