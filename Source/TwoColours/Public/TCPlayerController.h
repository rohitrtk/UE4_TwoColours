#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TCPlayerController.generated.h"

UCLASS()
class TWOCOLOURS_API ATCPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:

	/** The type of HUD player controller should be using */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
	TSubclassOf<class UUserWidget> wPlayerHUD;

	/** Current HUD player controller is using */
	class UUserWidget* PlayerHUD;

	virtual void BeginPlay() override;

};
