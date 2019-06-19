#pragma once

#include "CoreMinimal.h"
#include "PaperSpriteActor.h"
#include "TCObjectiveActor.generated.h"

UCLASS()
class TWOCOLOURS_API ATCObjectiveActor : public APaperSpriteActor
{
	GENERATED_BODY()
	
public:

	ATCObjectiveActor();

protected:

	virtual void BeginPlay();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCHealthComponent* HealthComponent;

	UFUNCTION()
	virtual void HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

};
