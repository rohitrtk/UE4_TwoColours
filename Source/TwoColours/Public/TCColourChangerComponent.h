#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TCColourComponent.h"
#include "TCColourChangerComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TWOCOLOURS_API UTCColourChangerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTCColourChangerComponent();

protected:
	virtual void BeginPlay() override;

public:	

	FORCEINLINE void ChangeColour(class UTCColourComponent* ColourComponent, const FLinearColor Colour) const
	{
		ColourComponent->SetCurrentColour(Colour);
		ColourComponent->UpdateSpriteMaterial();
	}

	UFUNCTION(BlueprintCallable)
	void FlipColour(class UTCColourComponent* ColourComponent) const;
};
