#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "SpartaGameState.generated.h"


UCLASS()
class SPARTAPROJECT_API ASpartaGameState : public AGameState
{
	GENERATED_BODY()

public:
	ASpartaGameState();

	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 SpawnedCoinCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Coin")
	int32 CollectedCoinCount;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	float WaveDuration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Level")
	int32 MaxLevels;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 CurrentWaveIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Wave")
	int32 MaxWaves;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> LevelMapNames;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
	TArray<USoundBase*> BGMs;

	FTimerHandle LevelTimerHandle;
	FTimerHandle HUDUpdateTimerHandle;
	FTimerHandle DebuffTimerHandle;

	UFUNCTION()
	void RefreshDebuffProgressBar();

	UFUNCTION(BlueprintCallable, Category = "Score")
	void AddScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "Level")
	void OnGameOver();
	UFUNCTION(BlueprintCallable, Category = "Sound")
	void PlayBGM();
	
	void StartLevel();
	void OnLevelTimeUp();
	void OnCoinCollected();
	void EndLevel();
	void UpdateHUD();
};
