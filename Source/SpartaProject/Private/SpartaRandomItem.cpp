#include "SpartaRandomItem.h"
#include "Components/SphereComponent.h" 
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "SpartaGameState.h"
#include "SpartaCharacter.h"


ASpartaRandomItem::ASpartaRandomItem()
{
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SKeletalMesh"));
	SkeletalMesh->SetupAttachment(Collision);

	DebuffDuration = 5;
}

void ASpartaRandomItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);
	ASpartaGameState* SpartaGameState = GetWorld() ? GetWorld()->GetGameState<ASpartaGameState>() : nullptr;
	if (SpartaGameState)
	{
		SpartaGameState->UpdateHUD();
	}
	
	int32 RandNum = FMath::RandRange(0, 2);
	
	
	switch (RandNum)
	{
	case 0:
		if (Activator && Activator->ActorHasTag(TEXT("Player")))
		{
			if (UWorld* World = GetWorld())
			{
				if (ASpartaGameState* GameState = World->GetGameState<ASpartaGameState>())
				{
					GameState->AddScore(150);
					GameState->OnCoinCollected();
				}
			}
			DestroyItem();
		}
		break;

	case 1:

		if (Activator && Activator->ActorHasTag(TEXT("Player")))
		{
			if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
			{
				PlayerCharacter->ApplySlowDebuff(DebuffDuration);
			}

			DestroyItem();
		}

		break;

	case 2:

		if (Activator && Activator->ActorHasTag(TEXT("Player")))
		{
			if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
			{
				PlayerCharacter->ApplyRotateCameraDebuff(DebuffDuration);
			}

			DestroyItem();
		}

		break;

	default:
		break;
	}
	
}
