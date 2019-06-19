#include "TCObjectiveActor.h"
#include "TwoColours.h"
#include "TCHealthComponent.h"
#include "PaperSpriteComponent.h"

ATCObjectiveActor::ATCObjectiveActor()
{
	this->GetRenderComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	this->GetRenderComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Overlap);
	this->GetRenderComponent()->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Overlap);

	this->HealthComponent = CreateDefaultSubobject<UTCHealthComponent>(TEXT("Health Component"));
}

void ATCObjectiveActor::BeginPlay()
{
}

void ATCObjectiveActor::HandleTakeDamage(UTCHealthComponent* HealthComp, int Lives,
	const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Log, TEXT("Objective current lives: %d"), Lives);

	if (Lives <= 0)
	{
		Destroy();
	}
}
