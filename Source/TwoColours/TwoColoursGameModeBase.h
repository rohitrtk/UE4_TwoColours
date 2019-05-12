#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TwoColoursGameModeBase.generated.h"

UCLASS()
class TWOCOLOURS_API ATwoColoursGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void RespawnPlayers();
	void RespawnPlayer(APlayerController* PlayerController);

};
