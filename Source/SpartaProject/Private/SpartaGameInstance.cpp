#include "SpartaGameInstance.h"
#include "Components/AudioComponent.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
	CurrentWaveIndex = 0;
	PlayerHealth = -1.f;
    CurrentSoundIndex = -1;
}

void USpartaGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}

void USpartaGameInstance::SavePlayerHealth(float Amount)
{
	PlayerHealth = Amount;
}

void USpartaGameInstance::PlayBGM(int32 SoundIndex)
{
    if (BGMs.IsValidIndex(SoundIndex))
    {
        USoundBase* NewBGM = BGMs[SoundIndex];
        if (CurrentSoundIndex == SoundIndex)
        {
            UE_LOG(LogTemp, Warning, TEXT("BGM is same. Keep playing."));
            return;
        }

        UE_LOG(LogTemp, Warning, TEXT("New BGM detected. Changing Music!"));
        if (CurrentBGMComponent)
        {
            CurrentBGMComponent->Stop();
        }

        CurrentBGMComponent = UGameplayStatics::CreateSound2D(this, NewBGM, 0.7f, 1.f, 0.f, nullptr, true, true);
        if (CurrentBGMComponent)
        {
            CurrentSoundIndex = SoundIndex;
            CurrentBGMComponent->Play();
        }
    }
}