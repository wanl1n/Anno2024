// Fill out your copyright notice in the Description page of Project Settings.


#include "Producer.h"

// Sets default values for this component's properties
UProducer::UProducer()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UProducer::BeginPlay()
{
	Super::BeginPlay();

	// ...
	this->resources.Init(0, 2);
}


// Called every frame
void UProducer::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	this->ticks += DeltaTime;
	if (this->ticks >= this->prodTime) {
		this->ticks = 0;

		if (this->producing) 
			this->Produce();
	}

	this->CheckProduction();
}

bool UProducer::HasStorageSpace(int index)
{
	return this->resources[index] < 3;
}

void UProducer::CheckProduction()
{
	if (this->IsReadyToProduce() &&
		!this->producing) {
		this->ticks = 0;
		this->producing = true;
	}
}

void UProducer::Produce()
{
	this->producing = false;

	this->storage += 2;

	if (type == 0) {
		this->resources[0]--;
		this->resources[1]--;
	}

	this->produce++;
}

bool UProducer::IsReadyToProduce()
{
	switch (this->type) {
		case 1: // from nature
			if (this->produce < 3)
				return true;
			else
				return false;
			break;
		case 0: // need resources
			if (this->resources[0] > 0 &&
				this->resources[1] > 0 && 
				this->produce < 3)
				return true;
			else
				return false;
			break;
		case 2: // sewing machine factory
			if (this->resources[0] > 0 &&
				this->resources[1] > 0)
				return true;
			else
				return false;
			break;
	}

	return false;
}

float UProducer::GetProductionProgress()
{
	float progress = 0.0f;

	if (this->producing) {
		progress = this->ticks / this->prodTime;
	}

	return progress;
}

