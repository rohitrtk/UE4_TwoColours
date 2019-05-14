#include "TCProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TCPlayerController.h"
#include "TCCharacter.h"

ATCProjectile::ATCProjectile()
{
	this->FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	RootComponent = this->FlipbookComponent;
	
	this->OverlapCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Collider"));
	this->OverlapCollider->SetupAttachment(RootComponent);

	this->ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
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
	ATCCharacter* potentialCharacter = Cast<ATCCharacter>(OtherActor);
	if (potentialCharacter)
	{
		ATCPlayerController* characterPlayerController = Cast<ATCPlayerController>(potentialCharacter->GetController());
		if (characterPlayerController) return;
	}

	UGameplayStatics::ApplyDamage(OtherActor, 1, OwningPlayerController, this, DamageType);
}

void ATCProjectile::SetOwningPlayerController(ATCPlayerController* controller)
{
	this->OwningPlayerController = controller;
}

ATCPlayerController* ATCProjectile::GetOwningPlayerController() 
{ 
	return this->OwningPlayerController; 
}
