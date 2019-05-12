#include "TwoColoursGameModeBase.h"
#include "Engine/World.h"

void ATwoColoursGameModeBase::RespawnPlayers()
{
	UE_LOG(LogTemp, Log, TEXT("Respawning Players!"));

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; It++)
	{
		APlayerController* playerController = It->Get();
		if (playerController && !playerController->GetPawn())
		{
			RestartPlayer(playerController);
		}
	}
}

void ATwoColoursGameModeBase::RespawnPlayer(APlayerController* PlayerController)
{
	// If there is no player controller or no pawn, return
	if (!PlayerController || !PlayerController->GetPawn()) return;

	RestartPlayer(PlayerController);
}
