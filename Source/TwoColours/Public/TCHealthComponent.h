#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TCHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FOnHealthChangedSignature, UTCHealthComponent*, HealthComp, int, Lives,
	const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(TC), meta=(BlueprintSpawnableComponent) )
class TWOCOLOURS_API UTCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTCHealthComponent();

protected:

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void HandleTakeDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, 
		class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Component")
	int StartingLives;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Component")
	int Lives;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Component")
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health Component")
	bool bCanTakeDamage;

public:	

	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnHealthChangedSignature OnHealthChanged;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetIsDead(const bool isDead) { this->bIsDead = isDead; }
	FORCEINLINE bool GetIsDead() const { return this->bIsDead; }
	FORCEINLINE void SetCanTakeDamage(const bool canTakeDamage) { this->bCanTakeDamage = canTakeDamage; }
	FORCEINLINE bool GetCanTakeDamage() const { return this->bCanTakeDamage; }
};
