#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "TCCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	CS_Idle			UMETA(DisplayName = "Idle"		),
	CS_Running		UMETA(DisplayName = "Running"	),
	CS_Jumping		UMETA(DisplayName = "Jumping"	),
	CS_Crouch		UMETA(DisplayName = "Crouch"	),
	CS_Death		UMETA(DisplayName = "Dead"		),
	CS_Hit			UMETA(DisplayName = "Hit"		),
	CS_Shoot		UMETA(DisplayName = "Shoot"		)
};

UCLASS()
class TWOCOLOURS_API ATCCharacter : public APaperCharacter
{
	GENERATED_BODY()

protected:

	/* Camera */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	/* Animation */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* RunningAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* JumpAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* DeathAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* HitAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* CrouchAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* ShootingAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	ECharacterStates CurrentCharacterState;

	/* Gameplay */

	bool bIsJumping;
	bool bIsDead;
	bool bIsShooting;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<class ATCProjectile> ProjectileClass;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	//class ATCProjectile* CurrentProjectile;

	/* Functions */

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void ManageDeath();
	void ManageAnimations();
	void UpdateAnimations();
	void MoveRight(float delta);
	void Fire();
	void StopFire();

	UFUNCTION()
	void HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void BlinkOnDamage();

	UFUNCTION()
	void ClearBlink();

private:

	int facingRight;
	float respawnTime = 3.f;

	FTimerHandle TimerHandle_Death;
	FTimerHandle TimerHandle_Blink;
	FTimerHandle TimerHandle_BlinkClear;

	virtual void Tick(float delta) override;

public:

	ATCCharacter();

	FORCEINLINE void SetCurrentCharacterState(ECharacterStates CharacterState) { this->CurrentCharacterState = CharacterState; }
	FORCEINLINE const ECharacterStates& GetCurrentCharacterState() const { return this->CurrentCharacterState; }
};
