// GPLv3

#include <algorithm>
#include "IG_EnemySpawner.h"
#include "IG_EnemyCharacter.h"
#include "IG_EnemyHealthBar.h"
#include "Blueprint/WidgetTree.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AIG_EnemyCharacter::AIG_EnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AIG_EnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	HealthBarWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), HealthBarWidget);
	HealthBarWidgetInstance->AddToViewport();
	HealthBarWidgetInstance->SetDesiredSizeInViewport(FVector2d(100, 16));
}

// Called every frame
void AIG_EnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector2d screen_pos;
	if (UGameplayStatics::GetPlayerController(GetWorld(), 0)->ProjectWorldLocationToScreen(GetActorLocation(), screen_pos))
	{
		screen_pos.X -= 50;
		screen_pos.Y -= 100;
		HealthBarWidgetInstance->SetPositionInViewport(screen_pos);
		HealthBarWidgetInstance->SetVisibility(ESlateVisibility::Visible);
	} else
	{
		HealthBarWidgetInstance->SetVisibility(ESlateVisibility::Hidden);
	}
}

// Called to bind functionality to input
void AIG_EnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AIG_EnemyCharacter::TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) {

	UE_LOG(LogTemp, Warning, TEXT("Took damage: %.2f"), Damage);
	int initial_health = CurrentHealth;
	CurrentHealth = std::clamp(CurrentHealth - static_cast<int>(Damage), 0, MaxHealth);

	if (HealthBarWidgetInstance)
	{
		float bar_percent = static_cast<float>(CurrentHealth) / static_cast<float>(MaxHealth);
		Cast<UIG_EnemyHealthBar>(HealthBarWidgetInstance)->HealthBar->SetPercent(bar_percent); 
	}
	
	if (CurrentHealth == 0) {
		Died();
	}

	return (CurrentHealth - initial_health);
}

void AIG_EnemyCharacter::Died() {
	HealthBarWidgetInstance->RemoveFromParent();
	HealthBarWidgetInstance = nullptr;
	spawner->CleanupEnemy(this);
	K2_DestroyActor();
}
