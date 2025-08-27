// Ninja Bear Studio Inc., all rights reserved.
#include "NinjaCommonUIFunctionLibrary.h"

#include "GameFramework/Actor.h"
#include "GameFramework/NinjaCommonHUD.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"

ANinjaCommonHUD* UNinjaCommonUIFunctionLibrary::TryGetHUD(const AActor* Owner)
{
	if (!IsValid(Owner))
	{
		return nullptr;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(Owner);
	if (IsValid(PlayerController))
	{
		return Cast<ANinjaCommonHUD>(PlayerController->GetHUD());
	}

	const APawn* Pawn = Cast<APawn>(Owner);
	if (IsValid(Pawn))
	{
		PlayerController = Cast<APlayerController>(Pawn->GetController());
		if (IsValid(PlayerController))
		{
			return Cast<ANinjaCommonHUD>(PlayerController->GetHUD());
		}
	}

	return nullptr;
}
