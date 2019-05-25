#include "TCColourChangerComponent.h"
#include "TCColourComponent.h"
#include "TwoColours.h"

UTCColourChangerComponent::UTCColourChangerComponent()
{
}

void UTCColourChangerComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UTCColourChangerComponent::ChangeColour(UTCColourComponent* ColourComponent)
{
	static int counter = 0;

	counter = counter > 2 ? 0 : counter++;
}
