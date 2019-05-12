#include "TCPlayerController.h"
#include "Blueprint/UserWidget.h"

void ATCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (!wPlayerHUD) return;
	
	this->PlayerHUD = CreateWidget<UUserWidget>(this, wPlayerHUD);

	if (PlayerHUD)
	{
		PlayerHUD->AddToViewport();
	}
}
