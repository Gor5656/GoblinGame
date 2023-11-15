// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NPCSpawner.generated.h"

UCLASS()
class GOBLINGAME_API ANPCSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANPCSpawner();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class ACharacter> EnemyClassCharacter;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float TimerUpdateRate = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	uint8 MaxNPCCount = 5;

	UPROPERTY(BlueprintReadWrite)
	uint8 NPCCount = 0;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnNPC();
	
	ACharacter* NPCCharacter;
	FTimerHandle SpawnTimer;

};
