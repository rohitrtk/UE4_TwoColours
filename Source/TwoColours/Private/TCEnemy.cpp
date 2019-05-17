#include "TCEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/CapsuleComponent.h"
#include "Components/ArrowComponent.h"
#include "PaperFlipbookComponent.h"

ATCEnemy::ATCEnemy()
{
	this->GetCapsuleComponent()->SetCapsuleHalfHeight(10.f);
	this->GetCapsuleComponent()->SetCapsuleRadius(10.f);
	this->GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Block);
	this->GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);

	this->GetSprite()->SetRelativeLocation(FVector(-2.f, 0.f, 1.f));

	this->GetCharacterMovement()->JumpZVelocity = 250.f;
	this->GetCharacterMovement()->AirControl = 2.f;
	this->GetCharacterMovement()->MaxWalkSpeed = 250.f;
	this->GetCharacterMovement()->MaxStepHeight = 6.f;
	
	this->GetArrowComponent()->ArrowSize = .3f;
	this->GetArrowComponent()->SetRelativeLocation(FVector(14.f, 0.f, 7.f));

	
}

void ATCEnemy::BeginPlay()
{
	Super::BeginPlay();
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

	ManageAnimations();
}

void ATCEnemy::ManageDeath()
{
}

void ATCEnemy::ManageAnimations()
{
	FVector velocity = this->GetVelocity();
	float velocitySq = velocity.SizeSquared();

	if (bIsDead)
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

	float direction = velocity.X;

	if (direction > 0.f)
	{
		Controller->SetControlRotation(FRotator(0.f, 0.f, 0.f));
	}
	else if (direction < 0.f)
	{
		Controller->SetControlRotation(FRotator(0.f, 180.f, 0.f));
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

void ATCEnemy::HandleTakeDamage(class UTCHealthComponent* HealthComp, int Lives, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
}
