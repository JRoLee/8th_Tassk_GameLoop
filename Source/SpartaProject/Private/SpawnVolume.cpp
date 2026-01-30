#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Engine/World.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));
	SpawningBox->SetupAttachment(Scene);

	ItemDataTable = nullptr;
}

FVector ASpawnVolume::GetRandomPointInVolume() const
{
	FVector BoxExtent = SpawningBox->GetScaledBoxExtent();
	FVector BoxOrigin = SpawningBox->GetComponentLocation();

	return BoxOrigin + FVector(
		FMath::FRandRange(-BoxExtent.X, BoxExtent.X),
		FMath::FRandRange(-BoxExtent.Y, BoxExtent.Y),
		FMath::FRandRange(-BoxExtent.Z, BoxExtent.Z)
	);
}

AActor* ASpawnVolume::SpawnRandomItem(int32 CurrentWaveIndex)
{
	if(FItemSpawnRow* SelectedRow = GetRandomItem(CurrentWaveIndex))
	{
		if (UClass* ActualClass = SelectedRow->ItemClass.Get())
		{
			return SpawnItem(ActualClass);
		}
	}

	return nullptr;
}

FItemSpawnRow* ASpawnVolume::GetRandomItem(int32 CurrentWaveIndex) const
{
	if (!ItemDataTable) return nullptr;
	
	TArray<FItemSpawnRow*> AllRows;
	static const FString ContextString(TEXT("ItemSpawnContext"));

	ItemDataTable->GetAllRows(ContextString, AllRows);

	if (AllRows.IsEmpty()) return nullptr;

	float TotalChance = 0.0f;
	for (const FItemSpawnRow* Row : AllRows)
	{
		if(Row && Row->WaveIndex == CurrentWaveIndex)
		{
			TotalChance += Row->Spawnchance;
		}
	}

	const float RandValue = FMath::FRandRange(0.0f, TotalChance);
	float AccumulatedChance = 0.0f;

	for (FItemSpawnRow* Row : AllRows)
	{
		if (Row && Row->WaveIndex == CurrentWaveIndex)
		{
			AccumulatedChance += Row->Spawnchance;
			if (RandValue <= AccumulatedChance)
			{
				return Row;
			}
		}
	}

	return nullptr;
}

AActor* ASpawnVolume::SpawnItem(TSubclassOf<AActor> ItemClass)
{
	if (!ItemClass) return nullptr;

	AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
		ItemClass,
		GetRandomPointInVolume(),
		FRotator::ZeroRotator
	);

	return SpawnedActor;
}


