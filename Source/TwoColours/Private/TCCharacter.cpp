#include "TCCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/InputComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "Camera/CameraComponent.h"
#include "PaperFlipbookComponent.h"
#include "Blueprint/UserWidget.h"
#include "TimerManager.h"
#include "TwoColoursGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TCHealthComponent.h"
#include "TCProjectile.h"
#include "TCPlayerController.h"

ATCCharacter::ATCCharacter()
{
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(10.f);
	this->GetCapsuleComponent()->SetCapsuleRadius(8.f);
	
	this->GetSprite()->SetRelativeLocation(FVector(0.f, 0.f, -12.f));

	this->GetCharacterMovement()->JumpZVelocity = 250.f;
	this->GetCharacterMovement()->AirControl = 2.f;
	this->GetCharacterMovement()->MaxWalkSpeed = 250.f;
	this->GetCharacterMovement()->MaxStepHeight = 6.f;

	this->SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	this->SpringArmComponent->SetupAttachment(this->RootComponent);
	this->SpringArmComponent->bAbsoluteRotation = true;
	this->SpringArmComponent->bDoCollisionTest = false;
	this->SpringArmComponent->TargetArmLength = 300.f;
	this->SpringArmComponent->RelativeRotation = FRotator(0.f, -90.f, 0.f);

	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	this->CameraComponent->bAutoActivate = true;
	this->CameraComponent->bUsePawnControlRotation = false;
	this->CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	this->CameraComponent->SetOrthoWidth(348.f);
	this->CameraComponent->SetupAttachment(this->SpringArmComponent);

	this->PSpawnArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Projectile Spawn Arrow"));
	this->PSpawnArrow->SetupAttachment(RootComponent);

	this->HealthComponent = CreateDefaultSubobject<UTCHealthComponent>(TEXT("Health Component"));

	this->FireTimerHandle.RateOfFire = 120.f;
	this->DamageImmuneTime = 2.f;

	this->bIsJumping = false;
	this->bIsShooting = false;

	this->facingRight = 0;
	this->respawnTime = 3.f;
}

void ATCCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->HealthComponent->OnHealthChanged.AddDynamic(this, &ATCCharacter::HandleTakeDamage);
	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATCCharacter::HandleOverlap);

	FireTimerHandle.TimeBetweenShots = 60.f / FireTimerHandle.RateOfFire;
}

void ATCCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveRight", this, &ATCCharacter::MoveRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ATCCharacter::Jump);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATCCharacter::StartFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATCCharacter::StopFire);
}

void ATCCharacter::MoveRight(float delta)
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), delta);
}

void ATCCharacter::Jump()
{
	Super::Jump();

	bIsJumping = true;
}
void ATCCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	if (bTookHit)
	{
		bKnockedDown = true;
	}

	bIsJumping = false;
}

void ATCCharacter::Tick(float delta)
{
	Super::Tick(delta);

	ManageAnimations();
}

void ATCCharacter::StartFire()
{
	if (!ProjectileClass ||
		bIsJumping ||
		CurrentCharacterState == ECharacterStates::CS_Running)
	{
		return;
	}

	float delay = FMath::Max(FireTimerHandle.LastFireTime + FireTimerHandle.TimeBetweenShots - GetWorld()->TimeSeconds, 0.f);

	GetWorldTimerManager().SetTimer(FireTimerHandle.TimerHandle_Handler, this, &ATCCharacter::Fire, FireTimerHandle.TimeBetweenShots, true, delay);
}

void ATCCharacter::Fire()
{
	if (bIsJumping || CurrentCharacterState == ECharacterStates::CS_Running) return;

	bIsShooting = true;

	GetCharacterMovement()->StopMovementImmediately();

	UE_LOG(LogTemp, Log, TEXT("%s firing Projectile"), *this->GetHumanReadableName());

	UArrowComponent* arrowComponent = GetArrowComponent();
	if (!arrowComponent) return;

	FActorSpawnParameters spawnParameters;
	spawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ATCProjectile* projectile =
		GetWorld()->SpawnActor<ATCProjectile>(ProjectileClass, PSpawnArrow->GetComponentTransform(), spawnParameters);
	projectile->SetOwningPlayerController(Cast<ATCPlayerController>(this->GetController()));

	FireTimerHandle.LastFireTime = GetWorld()->TimeSeconds;
}

void ATCCharacter::StopFire()
{
	bIsShooting = false;
	GetWorldTimerManager().ClearTimer(FireTimerHandle.TimerHandle_Handler);
}

void ATCCharacter::ManageAnimations()
{
	FVector playerVelocity = this->GetVelocity();
	float playerVelocitySq = playerVelocity.SizeSquared();

	if (HealthComponent->GetIsDead())
	{
		CurrentCharacterState = ECharacterStates::CS_Death;
	}
	else if (bKnockedDown)
	{
		CurrentCharacterState = ECharacterStates::CS_KnockDown;
	}
	else if (bTookHit)
	{
		CurrentCharacterState = ECharacterStates::CS_Hit;
	}
	else if (bIsJumping)
	{
		CurrentCharacterState = ECharacterStates::CS_Jumping;
	}
	else if (playerVelocitySq > 0.f)
	{
		CurrentCharacterState = ECharacterStates::CS_Running;
	}
	else if (bIsShooting)
	{
		CurrentCharacterState = ECharacterStates::CS_Shoot;
	}
	else
	{
		CurrentCharacterState = ECharacterStates::CS_Idle;
	}

	UpdateAnimations();

	float playerDirection = playerVelocity.X;

	if (playerDirection > 0.f)
	{
		Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
		this->facingRight = 1;
	}
	else if (playerDirection < 0.f)
	{
		Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));
		this->facingRight = -1;
	}
}

void ATCCharacter::UpdateAnimations()
{
	UPaperFlipbook* animation = nullptr;

	switch (CurrentCharacterState)
	{
	case ECharacterStates::CS_Death:
		animation = DeathAnimation;
		break;
	case ECharacterStates::CS_Hit:
		animation = HitAnimation;
		break;
	case ECharacterStates::CS_KnockDown:
		animation = KnockedDownAnimation;
		break;
	case ECharacterStates::CS_Shoot:
		animation = ShootingAnimation;
		break;
	case ECharacterStates::CS_Jumping:
		animation = JumpAnimation;
		break;
	case ECharacterStates::CS_Running:
		animation = RunningAnimation;
		break;
	case ECharacterStates::CS_Idle:
		animation = IdleAnimation;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Character was unable to identify animation!"));
		break;
	}

	if (animation && GetSprite()->GetFlipbook() != animation)
	{
		GetSprite()->SetFlipbook(animation);
	}
}

void ATCCharacter::StartBlink()
{
	UE_LOG(LogTemp, Log, TEXT("Starting blink"))

	this->HealthComponent->SetCanTakeDamage(false);
	GetWorldTimerManager().SetTimer(TimerHandle_Blink, this, &ATCCharacter::Blink, 0.1f, true);
	GetWorldTimerManager().SetTimer(TimerHandle_BlinkClear, this, &ATCCharacter::StopBlink, DamageImmuneTime, false);
}

void ATCCharacter::StopBlink()
{
	UE_LOG(LogTemp, Log, TEXT("Stopping blink"))

	GetWorldTimerManager().ClearTimer(TimerHandle_Blink);
	GetWorldTimerManager().ClearTimer(TimerHandle_BlinkClear);

	if (!GetSprite()->IsVisible()) GetSprite()->ToggleVisibility();

	this->HealthComponent->SetCanTakeDamage(true);
}

void ATCCharacter::Blink()
{
	GetSprite()->ToggleVisibility();
}

void ATCCharacter::ManageDeath()
{
	ATwoColoursGameModeBase* gameMode = Cast<ATwoColoursGameModeBase>(GetWorld()->GetAuthGameMode());
	if (gameMode)
	{
		UE_LOG(LogTemp, Log, TEXT("Attempting to respawn players through MangeDeath."));

		DetachFromControllerPendingDestroy();
		gameMode->RespawnPlayers();
	}

	Destroy();
}

void ATCCharacter::TakeHit()
{
	bTookHit = true;
	
	GetCharacterMovement()->Launch(FVector(-1 * facingRight * 200.f, 0.f, 200.f));

	GetWorldTimerManager().SetTimer(TimerHandle_EnableControlAfterHit, this, &ATCCharacter::EnableControl, DamageImmuneTime, false);
}

void ATCCharacter::EnableControl()
{
	APlayerController* playerController = Cast<APlayerController>(GetController());
	this->EnableInput(playerController);

	bTookHit = false;
	bKnockedDown = false;

	GetWorldTimerManager().ClearTimer(TimerHandle_EnableControlAfterHit);
}

void ATCCharacter::HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	FString healthString;
	healthString.AppendInt(Lives);

	UE_LOG(LogTemp, Log, TEXT("Number of Lives: %s"), *healthString);

	APlayerController* playerController = Cast<APlayerController>(GetController());
	this->DisableInput(playerController);
	
	if (Lives <= 0)
	{
		this->HealthComponent->SetIsDead(true);

		GetMovementComponent()->StopMovementImmediately();

		GetWorldTimerManager().SetTimer(TimerHandle_Death, this, &ATCCharacter::ManageDeath, respawnTime, false);

		return;
	}

	TakeHit();

	StartBlink();
}

void ATCCharacter::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Character is colliding"));
}
