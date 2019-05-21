#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "TCCharacter.generated.h"

UENUM(BlueprintType)
enum class ECharacterStates : uint8
{
	CS_Idle			UMETA(DisplayName = "Idle"				),
	CS_Running		UMETA(DisplayName = "Running"			),
	CS_Jumping		UMETA(DisplayName = "Jumping"			),
	CS_Death		UMETA(DisplayName = "Dead"				),
	CS_Hit			UMETA(DisplayName = "Hit"				),
	CS_KnockDown	UMETA(DisplayName = "Knocked Down"		),
	CS_Shoot		UMETA(DisplayName = "Shoot"				)
};

USTRUCT(BlueprintType)
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

USTRUCT(BlueprintType)
struct FCameraZoomHelper
{
	GENERATED_BODY()

public:

	/** Rate at which camera will zoom at */
	float ZoomInterval;
	
	/** Maximum ortho width of camera */
	float MaxOrthoWidth;

	/** Minimum ortho width of camera */
	float MinOrthoWidth;
};

UCLASS()
class TWOCOLOURS_API ATCCharacter : public APaperCharacter
{
	GENERATED_BODY()

protected:

	/* Camera */

	/** Starting length of camera boom */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	float CameraStartingDistance;

	/** Holds values related to the camera ortho width */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Camera")
	FCameraZoomHelper ZoomHelper;

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
	class UPaperFlipbook* KnockedDownAnimation;

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

	/** Did the character take a hit */
	bool bTookHit;

	/** Is the character knocked down? */
	bool bKnockedDown;

	/** Time in seconds that upon taking damage the character is immune to damage */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	float DamageImmuneTime;

	/** Manages characters health */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCHealthComponent* HealthComponent;

	/** Type of projectile to be shooting */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<class ATCProjectile> ProjectileClass;

	/** Arrow used for spawning projectiles */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UArrowComponent* PSpawnArrow;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UTCColourComponent* ColourComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UMaterialInterface* CurrentMaterialInterface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UMaterialInstanceDynamic* CurrentMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	struct FLinearColor CurrentColour;

	/* Functions */

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	void CameraZoom(int zoom);
	virtual void CameraZoomIn();
	virtual void CameraZoomOut();

	void ManageDeath();
	void ManageAnimations();
	void UpdateAnimations();
	
	void MoveRight(float delta);
	
	void StartFire();
	void StopFire();
	void Fire();

	void StartBlink();
	void StopBlink();
	void Blink();

	void TakeHit();
	void EnableControl();

	/** Handles */
	UFUNCTION()
	void HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
		class AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
			
private:

	int facingRight;
	float respawnTime;

	FFireTimerHandler FireTimerHandle;

	FTimerHandle TimerHandle_Death;
	FTimerHandle TimerHandle_Blink;
	FTimerHandle TimerHandle_BlinkClear;
	FTimerHandle TimerHandle_EnableControlAfterHit;

	virtual void Tick(float delta) override;

public:

	ATCCharacter();

	FORCEINLINE void SetCurrentCharacterState(ECharacterStates CharacterState) { this->CurrentCharacterState = CharacterState; }
	FORCEINLINE const ECharacterStates& GetCurrentCharacterState() const { return this->CurrentCharacterState; }

	FORCEINLINE const class UTCColourComponent* GetColourComponent() const { return this->ColourComponent; }
};
