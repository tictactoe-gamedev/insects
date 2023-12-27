// GPLv3

#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "IG_EnemySpawner.h"
#include "IG_GameMode.h"

// Sets default values
AIG_EnemySpawner::AIG_EnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Spawn Volume"));
    CollisionMesh->SetBoxExtent(FVector(32.f, 32.f, 32.f));
    SetRootComponent(CollisionMesh);
}

// Called when the game starts or when spawned
void AIG_EnemySpawner::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AIG_EnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto GameMode = Cast<AIG_GameMode>(GetWorld()->GetAuthGameMode());
	if (GameMode->IsPlayerAlive())
	{
		if (SpawnTimeCurrent > SpawnTimer) {
			Spawn();
			SpawnTimeCurrent = 0;
		} else {
			SpawnTimeCurrent += DeltaTime;
		}
	}
}

void AIG_EnemySpawner::Spawn() {

    if (spawned_enemies.Num() < MaxEnemies) {
        FVector spawn_pos = UKismetMathLibrary::RandomPointInBoundingBox(GetActorLocation(), CollisionMesh->GetScaledBoxExtent());
        FTransform spawn_transform = FTransform(FRotator(0.f), spawn_pos, FVector(1.f));
        FActorSpawnParameters spawn_info;
        AIG_EnemyCharacter* new_enemy = Cast<AIG_EnemyCharacter>(GetWorld()->SpawnActor(EnemyBase, &spawn_transform, spawn_info));
        
        new_enemy->spawner = this;
        spawned_enemies.Push(new_enemy);
    }
}

void AIG_EnemySpawner::CleanupEnemy(AIG_EnemyCharacter* enemy) {

    UE_LOG(LogTemp, Warning, TEXT("Cleaning up dead enemy: %s"), *(enemy->GetName()));
    for (const auto stored_enemy : spawned_enemies) {
        if (stored_enemy->GetName() == enemy->GetName()) {
            spawned_enemies.Remove(stored_enemy);
            break;
        }
    }
}