// Fill out your copyright notice in the Description page of Project Settings.

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
	
	bool bIsDead;

	/** Functions */
	
	virtual void BeginPlay() override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void ManageDeath();
	void ManageAnimations();
	void UpdateAnimations();
	
	void HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

private:

	virtual void Tick(float delta) override;

public:

	ATCEnemy();

};
