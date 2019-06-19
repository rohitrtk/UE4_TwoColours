#include "TCColourChanger.h"
#include "TCCharacter.h"
#include "TCColourChangerComponent.h"
#include "Components/BoxComponent.h"

ATCColourChanger::ATCColourChanger()
{
	this->BoxCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collider"));

	this->ColourChangerComponent = CreateDefaultSubobject<UTCColourChangerComponent>(TEXT("Colour Changer Component"));
}

void ATCColourChanger::BeginPlay()
{
	Super::BeginPlay();
	
	this->BoxCollider->OnComponentBeginOverlap.AddDynamic(this, &ATCColourChanger::HandleOverlap);

}

void ATCColourChanger::HandleOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATCCharacter* character = Cast<ATCCharacter>(OtherActor);

	if (!character)
	{
		return;
	}

	this->ColourChangerComponent->FlipColour(character->GetColourComponent());
}

