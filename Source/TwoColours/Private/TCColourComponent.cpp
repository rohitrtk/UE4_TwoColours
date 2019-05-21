#include "TCColourComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaperFlipbookComponent.h"
#include "TCCharacter.h"
#include "TCEnemy.h"
#include "TwoColours.h"

static int32 DebugPlayerColour = 0;
static int32 pDebugPlayerColour = 0;
FAutoConsoleVariableRef CVARDebugSetPlayerColour(TEXT("TC.SetPlayerColour"), DebugPlayerColour,
	TEXT("Changes current colour of player. 0 - Default, 1 - Red, 2 - Blue"), ECVF_Cheat);

UTCColourComponent::UTCColourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	this->CurrentColour = PLAYER_COLOUR_DEFAULT;
}

void UTCColourComponent::BeginPlay()
{
	Super::BeginPlay();

	this->CurrentColour = PLAYER_COLOUR_DEFAULT;

	UpdateSpriteMaterial();
}

void UTCColourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (pDebugPlayerColour == DebugPlayerColour)
	{
		return;
	}

	UpdateSpriteColour();
}

void UTCColourComponent::UpdateSpriteColour()
{
	pDebugPlayerColour = DebugPlayerColour;

	switch (DebugPlayerColour)
	{
	case 0:
		CurrentColour = PLAYER_COLOUR_DEFAULT;
		break;
	case 1:
		CurrentColour = PLAYER_COLOUR_RED;
		break;
	case 2:
		CurrentColour = PLAYER_COLOUR_BLUE;
		break;
	default:
		CurrentColour = PLAYER_COLOUR_DEFAULT;
		break;
	}

	UpdateSpriteMaterial();
}

void UTCColourComponent::UpdateSpriteMaterial()
{
	APaperCharacter* owner = Cast<APaperCharacter>(GetOwner());

	if(!owner)
	{
		return;
	}

	UPaperFlipbookComponent* sprite = owner->GetSprite();
	this->MaterialInterface = sprite->GetMaterial(0);
	this->DyanmicMaterialInstance = sprite->CreateDynamicMaterialInstance(0, MaterialInterface);

	if (DyanmicMaterialInstance)
	{
		DyanmicMaterialInstance->SetVectorParameterValue("Colour", CurrentColour);
	}
}
