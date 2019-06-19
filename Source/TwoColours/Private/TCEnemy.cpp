#include "TCEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "TCHealthComponent.h"
#include "TCColourComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Color.h"
#include "TwoColours.h"
#include "TCObjectiveActor.h"

ATCEnemy::ATCEnemy()
{
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(10.f);
	this->GetCapsuleComponent()->SetCapsuleRadius(10.f);
	this->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	this->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	this->GetSprite()->SetRelativeLocation(FVector(-2.f, 0.f, 1.f));
	
	this->GetCharacterMovement()->JumpZVelocity = 250.f;
	this->GetCharacterMovement()->AirControl = 2.f;
	this->GetCharacterMovement()->MaxWalkSpeed = 100.f;
	this->GetCharacterMovement()->MaxStepHeight = 6.f;
	
	this->GetArrowComponent()->ArrowSize = .3f;
	this->GetArrowComponent()->SetRelativeLocation(FVector(14.f, 0.f, 7.f));

	this->HealthComponent = CreateDefaultSubobject<UTCHealthComponent>(TEXT("Health Component"));
	this->ColourComponent = CreateDefaultSubobject<UTCColourComponent>(TEXT("Colour Component"));

	this->CurrentDirection = 1;
}

void ATCEnemy::BeginPlay()
{
	Super::BeginPlay();

	this->HealthComponent->OnHealthChanged.AddDynamic(this, &ATCEnemy::HandleTakeDamage);
	this->GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATCEnemy::HandleOverlap);

	this->ColourComponent->SetCurrentColour(PLAYER_COLOUR_RED);
	this->ColourComponent->UpdateSpriteMaterial();
}

void ATCEnemy::Jump()
{
	Super::Jump();
}

void ATCEnemy::Landed(const FHitResult& hit)
{
	Super::Landed(hit);
}

void ATCEnemy::Tick(float delta)
{
	Super::Tick(delta);

	Move();

	ManageAnimations();
}

void ATCEnemy::ManageDeath()
{
}

void ATCEnemy::ManageAnimations()
{
	FVector velocity = this->GetVelocity();
	float velocitySq = velocity.SizeSquared();

	if (HealthComponent->GetIsDead())
	{
		CurrentEnemyState = EEnemyStates::ES_Death;
	}
	else if (velocitySq > 0.f)
	{
		CurrentEnemyState = EEnemyStates::ES_Running;
	}
	else
	{
		CurrentEnemyState = EEnemyStates::ES_Idle;
	}

	UpdateAnimations();

	float direction = this->GetMovementComponent()->Velocity.X;

	if (direction > 0)
	{
		SetActorRotation(FRotator(0.f, 0.f, 0.f));
	}
	else if (direction < 0)
	{
		SetActorRotation(FRotator(0.f, 180.f, 0.f));
	}
}

void ATCEnemy::UpdateAnimations()
{
	UPaperFlipbook* animation = nullptr;

	switch (CurrentEnemyState)
	{
	case EEnemyStates::ES_Death:
		animation = DeathAnimation;
		break;
	case EEnemyStates::ES_Running:
		animation = RunningAnimation;
		break;
	case EEnemyStates::ES_Idle:
		animation = IdleAnimation;
		break;
	default:
		UE_LOG(LogTemp, Warning, TEXT("Enemy was unable to identify animation!"));
		break;
	}

	if (animation && GetSprite()->GetFlipbook() != animation)
	{
		GetSprite()->SetFlipbook(animation);
	}
}

void ATCEnemy::Move()
{
	AddMovementInput(FVector(1.f, 0.f, 0.f), CurrentDirection);
}

void ATCEnemy::HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Lives <= 0)
	{
		Destroy();
	}
}

void ATCEnemy::HandleOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	this->CurrentDirection *= -1;

	UGameplayStatics::ApplyDamage(OtherActor, 1, GetController(), this, DamageTypeClass);

	if (Cast<ATCObjectiveActor>(OtherActor))
	{
		Destroy();
	}
}
