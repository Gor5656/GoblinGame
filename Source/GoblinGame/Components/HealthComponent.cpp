// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetHealth(MaxHealth);
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* ComponentOwner = GetOwner();

	if (IsValid(ComponentOwner))
	{
		ComponentOwner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamageHandle);
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHealthComponent::SetHealth(float NewHealth)
{
	const float NextHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	const float HealthDelta = NextHealth - Health;

	Health = NextHealth;
	//	OnHealthChanged.Broadcast(Health, HealthDelta);
}

bool UHealthComponent::TryToAddHealth(int32 HealthAmount)
{
	if (Health == MaxHealth)
	{
		return false;
	}
	else
	{
		SetHealth(Health + HealthAmount);
		return true;
	}
}

void UHealthComponent::OnTakeAnyDamageHandle(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{

	if (Damage <= 0 || IsDead()) return;

	SetHealth(Health - Damage);

	GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
	else if (AutoHeal)
	{
		GetWorld()->GetTimerManager().SetTimer(HealTimerHandle, this, &UHealthComponent::OnHealTimerCalled, HealUpdateTime, true, HealDelay);
	}

}


void UHealthComponent::OnHealTimerCalled()
{
	if (GetHealth() < MaxHealth)
	{
		SetHealth(GetHealth() + HealModifier);
	}
	else
	{
		GetWorld()->GetTimerManager().ClearTimer(HealTimerHandle);
	}
}

