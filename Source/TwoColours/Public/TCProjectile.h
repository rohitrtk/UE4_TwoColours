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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UPaperFlipbookComponent* FlipbookComponent;

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

};
