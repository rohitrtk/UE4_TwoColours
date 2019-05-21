#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TCColourComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWOCOLOURS_API UTCColourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTCColourComponent();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	struct FLinearColor CurrentColour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UMaterialInterface* MaterialInterface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UMaterialInstanceDynamic* DyanmicMaterialInstance;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void UpdateSpriteMaterial();

	void UpdateSpriteColour();

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FORCEINLINE void SetCurrentColour(const FLinearColor* newColour) { this->CurrentColour = *newColour; }
	FORCEINLINE const FLinearColor* GetCurrentColour() const { return &this->CurrentColour; }
};
