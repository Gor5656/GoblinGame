// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GGCharacter.generated.h"

UCLASS()
class GOBLINGAME_API AGGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGGCharacter();

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	class UCapsuleComponent* SwordCapsuleComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimMontage* AttackAnimation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAnimInstance* AnimInstance;

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* RunAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UHealthComponent* HealthComponent;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bIsAttack = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Move(const FInputActionValue& Value);

	UFUNCTION()
	void Look(const FInputActionValue& Value);
	
	UFUNCTION()
	void Run(const FInputActionValue& Value);

	UFUNCTION()
	void StopRuning(const FInputActionValue& Value);

	UFUNCTION()
	void Attack(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	float GetMovementDirection();

	UFUNCTION()
	void OnCharacterDeath();
	
	UFUNCTION()
	void OnAttackAnimationEnd(class UAnimMontage* Montage, bool bInterrupted);

	UFUNCTION()
	void OnSwordCapsuleOverlaped(UPrimitiveComponent* OverlappedComponent,	//
								AActor* OtherActor,							//
								UPrimitiveComponent* OtherComponent,		//
								int32 OtherBodyIndex,						//
								bool bFromSweep,							//
								const FHitResult& Hit);						//

	UFUNCTION(BlueprintCallable)
	void SetSwordCollisionActive(bool bIsActive);

};
