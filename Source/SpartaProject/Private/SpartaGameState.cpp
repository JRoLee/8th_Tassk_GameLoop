#include "SpartaGameState.h"
#include "Kismet/GameplayStatics.h"
#include "SpartaGameInstance.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpawnVolume.h"
#include "CoinItem.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/UserWidget.h"


ASpartaGameState::ASpartaGameState()
{
	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;
	WaveDuration = 15;
	CurrentLevelIndex = 0;
	CurrentWaveIndex = 0;
	MaxWaves = 3;
	MaxLevels = 3;
}

void ASpartaGameState::BeginPlay()
{
	Super::BeginPlay();

	StartLevel();
	
	GetWorldTimerManager().SetTimer(
		HUDUpdateTimerHandle,
		this,
		&ASpartaGameState::UpdateHUD,
		0.1f,
		true
	);
}



void ASpartaGameState::AddScore(int32 Amount)
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			SpartaGameInstance->AddToScore(Amount);
		}
	}
}

void ASpartaGameState::StartLevel()
{
	PlayBGM();

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->ShowGameHUD();
		}
	}

	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentWaveIndex = SpartaGameInstance->CurrentWaveIndex;
			CurrentLevelIndex = SpartaGameInstance->CurrentLevelIndex;
		}
	}

	SpawnedCoinCount = 0;
	CollectedCoinCount = 0;

	TArray<AActor*> FoundVolumes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundVolumes);

	const int32 ItemToSpawn = 10;

	for (int32 i = 0; i < (MaxWaves-CurrentWaveIndex); i++)
	{
		for (int32 j = 0; j < ItemToSpawn; j++)
		{
			if (FoundVolumes.Num() > 0)
			{
				ASpawnVolume* SpawnVolume = Cast<ASpawnVolume>(FoundVolumes[0]);
				if (SpawnVolume)
				{
					AActor* SpawnedActor = SpawnVolume->SpawnRandomItem(CurrentWaveIndex);
					if (SpawnedActor && SpawnedActor->IsA(ACoinItem::StaticClass()))
					{
						SpawnedCoinCount++;
					}
				}
			}
		}
	}
	
	
	GetWorldTimerManager().SetTimer(
		LevelTimerHandle,
		this,
		&ASpartaGameState::OnLevelTimeUp,
		WaveDuration*(MaxWaves-CurrentWaveIndex+1),
		false
	);
		
}

void ASpartaGameState::OnLevelTimeUp()
{
	EndLevel();
}

void ASpartaGameState::OnCoinCollected()
{
	CollectedCoinCount++;
	UE_LOG(LogTemp, Warning, TEXT("Coin Collected: %d / %d"),
		CollectedCoinCount,
		SpawnedCoinCount);

	if (SpawnedCoinCount > 0 && CollectedCoinCount >= SpawnedCoinCount)
	{
		EndLevel();
	}
}


void ASpartaGameState::EndLevel()
{
	GetWorldTimerManager().ClearTimer(LevelTimerHandle);

	
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			CurrentWaveIndex++;
			SpartaGameInstance->CurrentWaveIndex = CurrentWaveIndex;
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(PlayerController->GetPawn()))
				{
					SpartaGameInstance->SavePlayerHealth(PlayerCharacter->GetHealth());
				}
			}

			if (CurrentWaveIndex >= MaxWaves)
			{
				CurrentWaveIndex = 0;
				SpartaGameInstance->CurrentWaveIndex = CurrentWaveIndex;
				CurrentLevelIndex++;
				SpartaGameInstance->CurrentLevelIndex = CurrentLevelIndex;
			}

		}
	}
	
	if (CurrentLevelIndex >= MaxLevels)
	{
		OnGameOver();
		return;
	}

	if(LevelMapNames.IsValidIndex(CurrentLevelIndex))
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelMapNames[CurrentLevelIndex]);
	}
	else
	{
		OnGameOver();
	}
}

void ASpartaGameState::OnGameOver()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			SpartaPlayerController->SetPause(true);
			SpartaPlayerController->ShowMainMenu(true);
		}
	}
}



void ASpartaGameState::UpdateHUD()
{
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (ASpartaPlayerController* SpartaPlayerController = Cast<ASpartaPlayerController>(PlayerController))
		{
			if (UUserWidget* HUDWidget = SpartaPlayerController->GetHUDWidget())
			{
				if (UTextBlock* TimeText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Time"))))
				{
					float RemainingTime = GetWorldTimerManager().GetTimerRemaining(LevelTimerHandle);
					TimeText->SetText(FText::FromString(FString::Printf(TEXT("Time: %.1f"), RemainingTime)));
				}

				if (UTextBlock* ScoreText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Score"))))
				{
					if (UGameInstance* GameInstance = GetGameInstance())
					{
						USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
						if (SpartaGameInstance)
						{
							ScoreText->SetText(FText::FromString(FString::Printf(TEXT("Score: %d"), SpartaGameInstance->TotalScore)));
						}
					}
				}
				if (UTextBlock* LevelIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Level"))))
				{
					
					LevelIndexText->SetText(FText::FromString(FString::Printf(TEXT("Level: %d"), CurrentLevelIndex+1)));
				}

				if (UTextBlock* WaveIndexText = Cast<UTextBlock>(HUDWidget->GetWidgetFromName(TEXT("Wave"))))
				{

					WaveIndexText->SetText(FText::FromString(FString::Printf(TEXT("Wave: %d"), CurrentWaveIndex + 1)));
				}

				if (UProgressBar* HealthBar = Cast<UProgressBar>(HUDWidget->GetWidgetFromName(TEXT("HealthBar"))))
				{
					ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(SpartaPlayerController->GetPawn());
					if (!FMath::IsNearlyZero(SpartaCharacter->GetMaxHealth()))
					{
						float Percent = SpartaCharacter->GetHealth() / SpartaCharacter->GetMaxHealth();
						HealthBar->SetPercent(Percent);
					}
				}
				if (ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(SpartaPlayerController->GetPawn()))
				{
					if (SpartaCharacter && (SpartaCharacter->bIsSlowDebuffOn || SpartaCharacter->bIsRotateDebuffOn))
					{
						if (!GetWorldTimerManager().IsTimerActive(DebuffTimerHandle))
						{
							GetWorldTimerManager().SetTimer(
								DebuffTimerHandle,
								this,
								&ASpartaGameState::RefreshDebuffProgressBar,
								0.05f,
								true
							);
						}
					}
				}
			}
		}
	}
}

void ASpartaGameState::RefreshDebuffProgressBar()
{
	ASpartaPlayerController* PlayerController = Cast<ASpartaPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PlayerController) return;
	UUserWidget* HUD = PlayerController->GetHUDWidget();
	ASpartaCharacter* SpartaCharacter = Cast<ASpartaCharacter>(PlayerController->GetPawn());
	if (!HUD || !SpartaCharacter) return;

	bool bAnyDebuffActive = false;

	if (UProgressBar* SlowBar = Cast<UProgressBar>(HUD->GetWidgetFromName(TEXT("SlowDebuffBar"))))
	{
		if (SpartaCharacter->bIsSlowDebuffOn)
		{
			float Remaining = GetWorldTimerManager().GetTimerRemaining(SpartaCharacter->SlowDebuffTimerHandle);
			float Percent = (SpartaCharacter->AppliedSlowDebuffDuration > 0) ? (Remaining / SpartaCharacter->AppliedSlowDebuffDuration) : 0.0f;
			SlowBar->SetPercent(Percent);
			SlowBar->SetVisibility(ESlateVisibility::Visible);
			bAnyDebuffActive = true;
		}
		else
		{
			SlowBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	if (UProgressBar* RotateBar = Cast<UProgressBar>(HUD->GetWidgetFromName(TEXT("RotateDebuffBar"))))
	{
		if (SpartaCharacter->bIsRotateDebuffOn)
		{
			float Remaining = GetWorldTimerManager().GetTimerRemaining(SpartaCharacter->RotateDebuffTimerHandle);
			float Percent = (SpartaCharacter->AppliedRotateDebuffDuration > 0) ? (Remaining / SpartaCharacter->AppliedRotateDebuffDuration) : 0.0f;
			RotateBar->SetPercent(Percent);
			RotateBar->SetVisibility(ESlateVisibility::Visible);
			bAnyDebuffActive = true;
		}
		else
		{
			RotateBar->SetVisibility(ESlateVisibility::Hidden);
		}
	}

	// 만약 두 디버프 모두 꺼졌다면 타이머 중지
	if (!bAnyDebuffActive)
	{
		GetWorldTimerManager().ClearTimer(DebuffTimerHandle);
	}
	
}

void ASpartaGameState::PlayBGM()
{
	USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GetGameInstance());
	if (SpartaGameInstance)
	{
		FString CurrentMapName = GetWorld()->GetMapName();

		if (CurrentMapName.Contains("MenuLevel"))
		{
			SpartaGameInstance->PlayBGM(0);
		}
		else
		{
			SpartaGameInstance->PlayBGM(1);
		}
	}
}
