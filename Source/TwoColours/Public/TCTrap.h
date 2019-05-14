#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TCTrap.generated.h"

UCLASS()
class TWOCOLOURS_API ATCTrap : public AActor
{
	GENERATED_BODY()
	
public:	

	ATCTrap();

protected:

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class USceneComponent* SceneComponent;

	/**  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	class UBoxComponent* OverlapCollider;

	virtual void BeginPlay() override;

	/**  */
	UFUNCTION(BlueprintNativeEvent)
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/**  */
	virtual void HandleOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, 
		UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:

	virtual void Tick(float DeltaTime) override;

};
