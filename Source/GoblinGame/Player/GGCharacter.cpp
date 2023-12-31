// Fill out your copyright notice in the Description page of Project Settings.


#include "GGCharacter.h"
#include "GGPlayerController.h"
#include "EnhancedInputComponent.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/EnhancedInputSubsystems.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "../Components/HealthComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Engine/DamageEvents.h"
#include "Animation/AnimInstance.h"

// Sets default values
AGGCharacter::AGGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SwordCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("SwordCapsule");
	SwordCapsuleComponent->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("RightHandSocket"));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");

}

// Called when the game starts or when spawned
void AGGCharacter::BeginPlay()
{
	Super::BeginPlay();

	AGGPlayerController* PlayerController = Cast<AGGPlayerController>(Controller);
	if (IsValid(PlayerController))
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (IsValid(Subsystem))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	if (IsValid(HealthComponent))
	{
		HealthComponent->OnDeath.AddDynamic(this, &AGGCharacter::OnCharacterDeath);
	}

	if (IsValid(SwordCapsuleComponent))
	{
		SwordCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
		SwordCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AGGCharacter::OnSwordCapsuleOverlaped);
	}
	AnimInstance = GetMesh()->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AnimInstance->OnMontageEnded.AddDynamic(this, &AGGCharacter::OnAttackAnimationEnd);
	}
}

// Called every frame
void AGGCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGGCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGGCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGGCharacter::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AGGCharacter::Jump);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Triggered, this, &AGGCharacter::Run);
		EnhancedInputComponent->BindAction(RunAction, ETriggerEvent::Completed, this, &AGGCharacter::StopRuning);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AGGCharacter::Attack);
	}

}

void AGGCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);

}

void AGGCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(-LookVector.X);
		AddControllerPitchInput(-LookVector.Y);
	}
}

void AGGCharacter::Run(const FInputActionValue& Value)
{

}

void AGGCharacter::StopRuning(const FInputActionValue& Value)
{

}

void AGGCharacter::Attack(const FInputActionValue& Value)
{	
	if (!bIsAttack)
	{
		bIsAttack = true;
		PlayAnimMontage(AttackAnimation);
	}
}

void AGGCharacter::OnAttackAnimationEnd(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == AttackAnimation)
	{
		bIsAttack = false;
	}
}

float AGGCharacter::GetMovementDirection()
{
	if (GetVelocity().IsZero()) return 0.f;

	const auto VelocityNormal = GetVelocity().GetSafeNormal();
	const auto AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const auto CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);

	const auto Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void AGGCharacter::OnCharacterDeath()
{
	GetCharacterMovement()->DisableMovement();
	SetLifeSpan(5.f);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	FTimerHandle TimerHandle;

	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([&]()
	{	
		if (IsValid(Controller))
		{
			Controller->ChangeState(NAME_Spectating);
		}
	});

	GetWorldTimerManager().SetTimer(TimerHandle, TimerDelegate, 2.f, false);
}

void AGGCharacter::OnSwordCapsuleOverlaped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if (IsValid(OtherActor) && OtherActor != this)
	{
		OtherActor->TakeDamage(50.f, FDamageEvent(), GetController(), this);
	}
}

void AGGCharacter::SetSwordCollisionActive(bool bIsActive)
{
	if (bIsActive)
	{
		SwordCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Overlap);
	}
	else
	{
		SwordCapsuleComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
}
