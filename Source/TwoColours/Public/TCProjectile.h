#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TCProjectile.generated.h"

UCLASS()
class TWOCOLOURS_API ATCProjectile : public AActor
{
	GENERATED_BODY()
	
public:	

	ATCProjectile();

protected:

	/** Propels projectile */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	/** Animation to be playing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UPaperFlipbookComponent* FlipbookComponent;

	/** Manages collisions */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class USphereComponent* OverlapCollider;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(BlueprintReadOnly, Category = "Gameplay")
	class ATCPlayerController* OwningPlayerController;

	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:	

	virtual void Tick(float DeltaTime) override;

	void SetOwningPlayerController(class ATCPlayerController* controller);
	class ATCPlayerController* GetOwningPlayerController();
};
