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

USTRUCT()
struct FFireTimerHandler
{
	GENERATED_BODY()

public:

	/** Rounds per minute */
	float RateOfFire;

	/** Last time weapon was fired */
	float LastFireTime;

	/** Time to wait between shots, 60/RateOfFire */
	float TimeBetweenShots;

	/** Timer handle */
	FTimerHandle TimerHandle_Handler;
};


UCLASS()
class TWOCOLOURS_API ATCCharacter : public APaperCharacter
{
	GENERATED_BODY()

protected:

	/* Camera */

	/** Camera component for character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComponent;

	/** Spring arm component attaches camera to player */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComponent;

	/* Animation */

	/** Animation for idle */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* IdleAnimation;

	/** Animation for running */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* RunningAnimation;

	/** Animation for jumping */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* JumpAnimation;

	/** Animation for dying */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* DeathAnimation;

	/** Animation for taking damage */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* HitAnimation;

	/** Animation for crouching */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* CrouchAnimation;

	/** Animation for shooting */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	class UPaperFlipbook* ShootingAnimation;

	/** The current animation to be playing */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animations")
	ECharacterStates CurrentCharacterState;

	/* Gameplay */

	/** Is the character jumping? */
	bool bIsJumping;

	/** Is the character dead? */
	bool bIsDead;

	/** Is the character shooting? */
	bool bIsShooting;

	/** Manages characters health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCHealthComponent* HealthComponent;

	/** Type of projectile to be shooting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<class ATCProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UArrowComponent* PSpawnArrow;

	/* Functions */

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void ManageDeath();
	void ManageAnimations();
	void UpdateAnimations();
	void MoveRight(float delta);
	void StartFire();
	void StopFire();
	void Fire();

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

	FFireTimerHandler FireTimerHandle;

	FTimerHandle TimerHandle_Death;
	FTimerHandle TimerHandle_Blink;
	FTimerHandle TimerHandle_BlinkClear;

	virtual void Tick(float delta) override;

public:

	ATCCharacter();

	FORCEINLINE void SetCurrentCharacterState(ECharacterStates CharacterState) { this->CurrentCharacterState = CharacterState; }
	FORCEINLINE const ECharacterStates& GetCurrentCharacterState() const { return this->CurrentCharacterState; }
};
