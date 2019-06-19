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

void UTCColourChangerComponent::FlipColour(UTCColourComponent* ColourComponent) const
{
	FLinearColor newColour;
	const FLinearColor& currentColour = ColourComponent->GetCurrentColour();

	// Set new to red or blue if the current colour is default or not red or blue
	if ((currentColour == PLAYER_COLOUR_DEFAULT) ||
		((currentColour != PLAYER_COLOUR_RED) && (currentColour != PLAYER_COLOUR_BLUE)))
	{
		newColour = FMath::RandRange(0, 1) == 0 ? PLAYER_COLOUR_RED : PLAYER_COLOUR_BLUE;
	}
	else
	{
		newColour = currentColour == PLAYER_COLOUR_BLUE ? PLAYER_COLOUR_RED : PLAYER_COLOUR_BLUE;
	}

	ChangeColour(ColourComponent, newColour);
}
