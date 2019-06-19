#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TCColourChanger.generated.h"

UCLASS()
class TWOCOLOURS_API ATCColourChanger : public AActor
{
	GENERATED_BODY()
	
public:	
	ATCColourChanger();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UBoxComponent* BoxCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCColourChangerComponent* ColourChangerComponent;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
