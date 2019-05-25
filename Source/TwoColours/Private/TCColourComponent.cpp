#include "TCColourComponent.h"
#include "Materials/MaterialInterface.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "PaperFlipbookComponent.h"
#include "TCCharacter.h"
#include "TCEnemy.h"
#include "TCProjectile.h"
#include "TwoColours.h"

//static int32 CurrentPlayerColour = 0;
//static int32 DebugPlayerColour = 0;
//FAutoConsoleVariableRef CVARDebugSetPlayerColour(TEXT("TC.SetPlayerColour"), DebugPlayerColour,
//	TEXT("Changes current colour of player. 0 - Default, 1 - Red, 2 - Blue"), ECVF_Cheat);

UTCColourComponent::UTCColourComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.SetTickFunctionEnable(true);

	this->CurrentColour = PLAYER_COLOUR_DEFAULT;
}

void UTCColourComponent::BeginPlay()
{
	Super::BeginPlay();

	UpdateSpriteMaterial();
}

void UTCColourComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
}

void UTCColourComponent::UpdateSpriteMaterial()
{
	APaperCharacter* ownerCharacter = nullptr;
	ATCProjectile*	 ownerProjectile = nullptr;

	ownerCharacter = Cast<APaperCharacter>(GetOwner());
	if (ownerCharacter)
	{
		SetOwnerMaterial(ownerCharacter->GetSprite());
	}
	else
	{
		ownerProjectile = Cast<ATCProjectile>(GetOwner());
		if (ownerProjectile)
		{
			ATCCharacter* character = Cast<ATCCharacter>(ownerProjectile->GetOwner());
			if (character)
			{
				this->CurrentColour = character->GetColourComponent()->GetCurrentColour();
				SetOwnerMaterial(ownerProjectile->GetFlipbookComponent());
			}
		}
	}
}
