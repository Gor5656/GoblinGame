// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, float);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GOBLINGAME_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	UFUNCTION(BlueprintCallable)
	float GetHealth() const {return Health;};

	UFUNCTION(BlueprintCallable)
    float GetHealthPercent() const { return Health / MaxHealth; };

	UFUNCTION(BlueprintCallable)
    void SetHealth(float NewHealth);

    UFUNCTION(BlueprintCallable)
    bool IsDead() const {return Health <= 0.f;};

	bool TryToAddHealth(int32 HealthAmount);

	FOnDeath OnDeath;
	FOnHealthChanged OnHealthChanged;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UFUNCTION()
    void OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
		class AController* InstigatedBy,AActor* DamageCauser);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0", ClampMax = "10"))
	float MaxHealth = 100.f;

	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (ClampMin = "0", ClampMax = "10"))
	bool AutoHeal = true;

	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (ClampMin = "0", ClampMax = "10"))
	float HealUpdateTime = 0.3f;	

	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (ClampMin = "0", ClampMax = "10"))
	float HealDelay = 3.f;

	UPROPERTY(EditDefaultsOnly, Category = "Heal", meta = (ClampMin = "0", ClampMax = "10"))
	float HealModifier = 1.f;

	FTimerHandle HealTimerHandle;

    void OnHealTimerCalled();


private:

	UPROPERTY()
	float Health;

};

