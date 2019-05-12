#include "TCProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "PaperFlipbookComponent.h"
#include "PaperSpriteComponent.h"

ATCProjectile::ATCProjectile()
{
	this->FlipbookComponent = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("Sprite"));
	RootComponent = this->FlipbookComponent;
	
	this->ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement Component"));
}

void ATCProjectile::BeginPlay()
{
	Super::BeginPlay();	
}

void ATCProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

