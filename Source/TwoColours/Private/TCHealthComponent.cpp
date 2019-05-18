#include "TCHealthComponent.h"
#include "GameFramework/Actor.h"
#include "TCCharacter.h"

UTCHealthComponent::UTCHealthComponent()
{
	this->StartingLives = 3;

	this->bIsDead = false;
	this->bCanTakeDamage = true;
}

void UTCHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AActor* owner = GetOwner();
	if (owner)
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UTCHealthComponent::HandleTakeDamage);
	}

	this->Lives = this->StartingLives;
}

void UTCHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTCHealthComponent::HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,
	class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0.f || !bCanTakeDamage || bIsDead) return;

	Lives = FMath::Clamp(Lives - static_cast<int>(Damage), 0, StartingLives);

	this->OnHealthChanged.Broadcast(this, Lives, DamageType, InstigatedBy, DamageCauser);
}
