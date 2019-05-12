#include "TCTrap.h"
#include "Components/BoxComponent.h"

ATCTrap::ATCTrap()
{
	PrimaryActorTick.bCanEverTick = true;

	this->SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene Component"));
	RootComponent = this->SceneComponent;

	this->OverlapCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));
	this->OverlapCollider->SetupAttachment(RootComponent);
}

void ATCTrap::BeginPlay()
{
	Super::BeginPlay();

	this->OverlapCollider->OnComponentBeginOverlap.AddDynamic(this, &ATCTrap::HandleOverlap);
}

void ATCTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATCTrap::HandleOverlap_Implementation(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
