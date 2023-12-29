// GPLv3

#include "IG_PlayerCharacter.h"
#include "IG_PlayerHealthBar.h"
#include "IG_PlayerHud.h"
#include "../IG_GameMode.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EnhancedInputComponent.h"
#include "ToolContextInterfaces.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AIG_PlayerCharacter::AIG_PlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIG_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Configure the player controller
	PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
		{
			// Assign input map
			Subsystem->AddMappingContext(InputContext, 0);
		}
		else
		{
			UE_LOG(LogPlayerController, Error, TEXT("Failed to acquire local player input subsystem"));
		}

		// Enable mouse cursor
		PlayerController->SetShowMouseCursor(true);

		// Set the default walk speed
		DefaultPlayerMoveSpeed = GetCharacterMovement()->MaxWalkSpeed;
	}
	else
	{
		UE_LOG(LogPlayerController, Error, TEXT("Failed to acquire player controller"));
		return;
	}

	// Locate our weapon in the actor tree
	TArray<USkeletalMeshComponent*> Components;
	GetComponents<USkeletalMeshComponent>(Components, true);
	
	for (USkeletalMeshComponent * Comp : Components)
	{
		// Getting by name isn't perfect but navigating this stuff is hard :D
		if (Comp->GetFName() == FName("sword"))
		{
			// Cache the weapon
			Weapon = Comp;
		}
		else if (Comp->GetFName() == FName("CharacterMesh0"))
		{
			// Cache player mesh & animator
			PlayerMesh = Comp;
			PlayerAnimator = Comp->GetAnimInstance();
		}
	}

	// Cache vars
	GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	PlayerHud = Cast<AIG_PlayerHud>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
 	PlayerHealthBar = Cast<UIG_PlayerHealthBar>(PlayerHud->HealthBarWidgetInstance);
}

// Called every frame
void AIG_PlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Skip ticks if dead
	if (Dead)
	{
		return;
	}

	// Get location under the mouse cursor
	FHitResult HitResult;
	PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), false, HitResult);
	if (HitResult.IsValidBlockingHit())
	{
		// Rotate the player towards the cursor
		PlayerController->SetControlRotation((HitResult.ImpactPoint - GetActorLocation()).Rotation());		
	}

	// Run hit detection if we are attacking
	if (IsAttacking)
	{
		// If we hit something
		if (const auto HitActor = DoHitDetection())
		{
			// Check if the actor is already in the list
			if (ActorsToIgnore.Find(HitActor) == INDEX_NONE)
			{
				// If not, add it to the list for next time
				ActorsToIgnore.AddUnique(Cast<AActor>(HitActor));
				
				UE_LOG(LogTemp, Warning, TEXT("Hit: %s"), *(HitActor->GetName()));

				// Apply the damage to the hit enemy
				UGameplayStatics::ApplyDamage(
					HitActor,
					PlayerDamage,
					GetController(),
					this,
					UDamageType::StaticClass()
				);
			}
		}
	}
}

// Called to bind functionality to input
void AIG_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Setup enhanced input
	auto EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(InputActionMove, ETriggerEvent::Triggered, this, &AIG_PlayerCharacter::OnMove);
		EnhancedInputComponent->BindAction(InputActionSprint, ETriggerEvent::Triggered, this, &AIG_PlayerCharacter::OnSprintStart);
		EnhancedInputComponent->BindAction(InputActionSprint, ETriggerEvent::Completed, this, &AIG_PlayerCharacter::OnSprintEnd);
		EnhancedInputComponent->BindAction(InputActionAttack, ETriggerEvent::Triggered, this, &AIG_PlayerCharacter::OnAttackInput);
	}
	else
	{
		UE_LOG(LogInput, Error, TEXT("Error configuring input system"));
	}
}

AIG_EnemyCharacter* AIG_PlayerCharacter::DoHitDetection() {

	// Locate ends of the weapon
	FVector StartLocation = Weapon->GetSocketLocation("WeaponStart");
	FVector EndLocation = Weapon->GetSocketLocation("WeaponEnd");

	// Generate array of object types to hit
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Reserve(1);
    ObjectTypes.Emplace(ECollisionChannel::ECC_Pawn);

	// Fire the collision trace
    FHitResult HitResult = FHitResult(ForceInit);
    bool Hit = UKismetSystemLibrary::SphereTraceSingleForObjects(
            GetWorld(),
            StartLocation,
            EndLocation,
            20.f, // Seems a reasonable diameter for trace capsule
            ObjectTypes,
            false,
            ActorsToIgnore,		// Our dynamic ignore list
            EDrawDebugTrace::Type::None, // Use EDrawDebugTrace::Type::ForDuration for debugging
            HitResult,
            true,
            FLinearColor(255,0,0,255),
            FLinearColor(0,255,0,255),
            1.f
            );

	// Early return if we didn't hit anything
    if (!Hit)
    {
	    return {};
    }
	
    // return the actor that was hit
    return Cast<AIG_EnemyCharacter>(HitResult.GetActor());
}

void AIG_PlayerCharacter::ClearHitDetection() {
	// Reset the ignored list
    ActorsToIgnore.Empty();
	// Add self always
    ActorsToIgnore.Push(this);
}

float AIG_PlayerCharacter::TakeDamage(const float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Player took %.2f damage"), Damage);
	
	// Grab initial health
	const int InitialHealth = CurrentHealth;

	// Calculate the result of the damage to the enemy health
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);

	// Don't kill the player, just print a message
	if (CurrentHealth == 0) {
		UE_LOG(LogTemp, Warning, TEXT("Player died"), Damage);
		Dead = true;

		// broadcast to delegates that we died
		OnPlayerDeathDelegate.Broadcast();
		
	}
	
	// Calc and send the new percentage to the health bar
	const float BarPercent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
	PlayerHealthBar->HealthBar->SetPercent(BarPercent);
	
	// Return the difference (damage done);
	return (CurrentHealth - InitialHealth);
}

void AIG_PlayerCharacter::OnMove(const FInputActionValue& Value)
{
	// Can't move if you're dead
	if (Dead)
	{
		return;
	}
	
	// Get the movement vector from the action
	FVector MoveVector = Value.Get<FVector>();

	// Attempt to normalize
	if (MoveVector.Normalize())
	{
		// Apply movement
		AddMovementInput(MoveVector, 1.f);
	}
	else
	{
		UE_LOG(LogPlayerController, Warning, TEXT("Failed normalize movement"));
	}
}

void AIG_PlayerCharacter::OnSprintStart(const FInputActionValue& Value)
{
	// TODO This will get called constantly while sprint key is down
	// Couldn't find a way to get the input to fire once on down and once on up

	// Apply sprint modifier to speed
	GetCharacterMovement()->MaxWalkSpeed = DefaultPlayerMoveSpeed * SprintModifier;
}

void AIG_PlayerCharacter::OnSprintEnd(const FInputActionValue& Value)
{
	// Return to regular speed
	GetCharacterMovement()->MaxWalkSpeed = DefaultPlayerMoveSpeed;
}

void AIG_PlayerCharacter::OnAttackInput(const FInputActionValue& Value)
{
	// Bail out if already attacking
	if (IsAttacking)
	{
		return;
	}
	
	// Clear the previous hit records
	ClearHitDetection();
	
	// Setup the attack montage
	const float MontageLength = PlayerAnimator->Montage_Play(
		AttackAnimMontage,
		1.0f,
		EMontagePlayReturnType::MontageLength,
		0.f
	);

	// If the montage is valid
	if (MontageLength > 0.f)
	{
		// Bind callbacks
		PlayerAnimator->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &AIG_PlayerCharacter::OnAttackStart);
		PlayerAnimator->OnPlayMontageNotifyEnd.AddUniqueDynamic  (this, &AIG_PlayerCharacter::OnAttackEnd);
	}
}
