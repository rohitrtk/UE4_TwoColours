#include "TCProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TCPlayerController.h"
#include "TCColourComponent.h"
#include "TCCharacter.h"
#include "TCEnemy.h"
#include "TwoColours.h"

ATCProjectile::ATCProjectile()
{
	this->FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	RootComponent = this->FlipbookComponent;
	
	this->OverlapCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	this->OverlapCollider->SetSphereRadius(4.f);
	this->OverlapCollider->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	this->OverlapCollider->SetupAttachment(RootComponent);

	this->ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));

	this->ColourComponent = CreateDefaultSubobject<UTCColourComponent>(TEXT("Colour Component"));
}

void ATCProjectile::BeginPlay()
{
	Super::BeginPlay();	

	this->OverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &ATCProjectile::HandleOverlap);
}

void ATCProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATCProjectile::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATCEnemy* enemy = Cast<ATCEnemy>(OtherActor);
	if (enemy)
	{
		if (enemy->GetColourComponent()->GetCurrentColour() != this->ColourComponent->GetCurrentColour())
		{
			UE_LOG(LogTemp, Log, TEXT("Not applying damage"));
			Destroy();
			return;
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Applying damage"));
	UGameplayStatics::ApplyDamage(OtherActor, 1, nullptr, this, DamageType);

	Destroy();
}
