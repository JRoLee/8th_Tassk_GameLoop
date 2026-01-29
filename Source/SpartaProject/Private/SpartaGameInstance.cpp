#include "SpartaGameInstance.h"

USpartaGameInstance::USpartaGameInstance()
{
	TotalScore = 0;
	CurrentLevelIndex = 0;
	PlayerHealth = 0;
}

void USpartaGameInstance::AddToScore(int32 Amount)
{
	TotalScore += Amount;
}

void USpartaGameInstance::SavePlayerHealth(float Amount)
{
	PlayerHealth = Amount;
}
