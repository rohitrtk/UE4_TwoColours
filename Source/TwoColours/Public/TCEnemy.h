#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "TCEnemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyStates : uint8
{
	ES_Idle		UMETA(DisplayName = "Idle"		),
	ES_Running	UMETA(DisplayName = "Running"	),
	ES_Death	UMETA(DisplayName = "Death"		)	
};

UCLASS()
class TWOCOLOURS_API ATCEnemy : public APaperCharacter
{
	GENERATED_BODY()
	
protected:
	/** Animations */

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UPaperFlipbook* RunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animations")
	EEnemyStates CurrentEnemyState;

	/** Gameplay */
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<class UDamageType> DamageTypeClass;

	UFUNCTION()
	virtual void HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

	
	UFUNCTION(BlueprintNativeEvent)
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void HandleOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
			
	/** Functions */
	
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void ManageDeath();
	void ManageAnimations();
	void UpdateAnimations();

private:

	virtual void Tick(float delta) override;

public:

	ATCEnemy();
};
