// GPLv3

#include "IG_EnemySpawner.h"
#include "../IG_GameMode.h"
#include "../Player/IG_PlayerCharacter.h"

#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AIG_EnemySpawner::AIG_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// The area we will be spawning in
    CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Spawn Volume"));
	// These are the defaults you get when you create a box collider so might as well use them here too
	const FVector DefaultExtents{32.f, 32.f, 32.f}; 
    CollisionMesh->SetBoxExtent(DefaultExtents);
    SetRootComponent(CollisionMesh);
}

// Called when the game starts or when spawned
void AIG_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Cache frequent flyers
	GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	const auto PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	const auto PlayerPawn = PlayerController->GetPawn();
	const auto PlayerCharacter = Cast<AIG_PlayerCharacter>(PlayerPawn);

	// Subscribe to player death event
	PlayerCharacter->OnPlayerDeathDelegate.AddDynamic(this, &AIG_EnemySpawner::OnPlayerDeath);
}

// Called every frame
void AIG_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Check the spawn timer
	if (SpawnTimeCurrent > SpawnTimer) {
		// Spawn enemy & reset timer
		Spawn();
		SpawnTimeCurrent = 0;
	} else {
		// Advance the timer
		SpawnTimeCurrent += DeltaTime;
	}
}

void AIG_EnemySpawner::Spawn() {

	// Check we haven't spawned our max number of enemies
    if (SpawnedEnemies.Num() < MaxEnemies) {
    	// Get a random position inside our box volume
        const FVector SpawnPos = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), CollisionMesh->GetScaledBoxExtent());
    	// Set a basic transform
        const FTransform SpawnTransform = FTransform(FRotator(0.f), SpawnPos, FVector(1.f));
    	// We have no special parameters
        const FActorSpawnParameters SpawnInfo;
        AIG_EnemyCharacter* NewEnemy = Cast<AIG_EnemyCharacter>(GetWorld()->SpawnActor(EnemyBase, &SpawnTransform, SpawnInfo));

    	// Subscribe to enemy death so we can remove it's useless corpse
    	NewEnemy->OnEnemyDeathDelegate.AddDynamic(this, &AIG_EnemySpawner::CleanupEnemy);
    	
    	// Register the new enemy in our collection
        SpawnedEnemies.Push(NewEnemy);
    }
}

void AIG_EnemySpawner::CleanupEnemy(AIG_EnemyCharacter* Enemy) {

    UE_LOG(LogTemp, Warning, TEXT("Cleaning up dead enemy: %s"), *(Enemy->GetName()));

	// Get the enemy from our collection
    for (const auto StoredEnemy : SpawnedEnemies) {
    	// TODO: Couldn't find a clean way to find the enemy in the array
    	// By name was the best I could come up with. Might need to migrate to a map?
        if (StoredEnemy->GetName() == Enemy->GetName()) {
            SpawnedEnemies.Remove(StoredEnemy);
            break;
        }
    }
}