#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TCColourComponent.generated.h"

#define DEBUG_COLOUR_DEFAULT	0
#define DEBUG_COLOUR_RED		1
#define DEBUG_COLOUR_BLUE		2

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWOCOLOURS_API UTCColourComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UTCColourComponent();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gameplay")
	struct FLinearColor CurrentColour;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UMaterialInterface* MaterialInterface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gameplay")
	class UMaterialInstanceDynamic* DyanmicMaterialInstance;

	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void UpdateSpriteMaterial();

	FORCEINLINE void SetCurrentColour(const FLinearColor newColour) { this->CurrentColour = newColour; }
	FORCEINLINE const FLinearColor& GetCurrentColour() const { return this->CurrentColour; }

private:

	template <class T>
	void SetOwnerMaterial(T* owner)
	{
		MaterialInterface = owner->GetMaterial(0);
		DyanmicMaterialInstance = owner->CreateDynamicMaterialInstance(0, MaterialInterface);

		if (DyanmicMaterialInstance)
		{
			DyanmicMaterialInstance->SetVectorParameterValue("Colour", CurrentColour);
		}
	}

};
