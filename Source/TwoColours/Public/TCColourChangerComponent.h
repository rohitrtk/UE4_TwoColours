#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UFUNCTION(BlueprintCallable)
	void ChangeColour(class UTCColourComponent* ColourComponent);

};
