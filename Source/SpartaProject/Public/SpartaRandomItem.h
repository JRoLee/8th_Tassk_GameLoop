#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "SpartaRandomItem.generated.h"

UCLASS()
class SPARTAPROJECT_API ASpartaRandomItem : public ABaseItem
{
	GENERATED_BODY()
	
public:
	ASpartaRandomItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Debuff")
	float DebuffDuration;

	virtual void ActivateItem(AActor* Activator) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item|Components")
	USkeletalMeshComponent* SkeletalMesh;
};
