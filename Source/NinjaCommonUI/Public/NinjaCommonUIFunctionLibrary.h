// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "NinjaCommonUIFunctionLibrary.generated.h"

class ANinjaCommonHUD;
/**
 * Set of helper functions for the Common UI layer.
 */
UCLASS()
class NINJACOMMONUI_API UNinjaCommonUIFunctionLibrary : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()

public:

	/**
	 * Attempts to retrieve the HUD for a given actor (Pawn, Character, Controller).
	 */
	UFUNCTION(BlueprintPure, Category = "Ninja Common UI")
	static ANinjaCommonHUD* TryGetHUD(const AActor* Owner);
	
};
