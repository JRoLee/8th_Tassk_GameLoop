#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SpartaGameInstance.generated.h"


UCLASS()
class SPARTAPROJECT_API USpartaGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	USpartaGameInstance();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 TotalScore;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentLevelIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "GameData")
	int32 CurrentWaveIndex;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "PlayerData")
	float PlayerHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SoundData")
	int32 CurrentSoundIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SoundData")
	TArray<USoundBase*> BGMs;
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category = "SoundData")
	UAudioComponent* CurrentBGMComponent;

	UFUNCTION(BlueprintCallable, Category = "GameData")
	void AddToScore(int32 Amount);
	UFUNCTION(BlueprintCallable, Category = "GameData")
	void SavePlayerHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category = "SoundData")
	void PlayBGM(int32 SoundIndex);
};
