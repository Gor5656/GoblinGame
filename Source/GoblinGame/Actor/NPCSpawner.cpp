// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCSpawner.h"
#include "GameFramework/Character.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"

// Sets default values
ANPCSpawner::ANPCSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANPCSpawner::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ANPCSpawner::SpawnNPC, TimerUpdateRate, true, 1.f);
	
}

// Called every frame
void ANPCSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
