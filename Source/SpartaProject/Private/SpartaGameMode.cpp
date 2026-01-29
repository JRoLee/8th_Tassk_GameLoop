#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"
#include "SpartaGameInstance.h"

ASpartaGameMode::ASpartaGameMode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}

void ASpartaGameMode::BeginPlay()
{
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		USpartaGameInstance* SpartaGameInstance = Cast<USpartaGameInstance>(GameInstance);
		if (SpartaGameInstance)
		{
			if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
			{
				if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(PlayerController->GetPawn()))
				{
					if (SpartaGameInstance->PlayerHealth > 0)
					{
						PlayerCharacter->ApplyHealth(SpartaGameInstance->PlayerHealth);
					}					
				}
			}
		}
	}
}
