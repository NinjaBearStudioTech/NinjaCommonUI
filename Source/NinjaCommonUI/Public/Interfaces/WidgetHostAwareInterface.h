// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WidgetHostAwareInterface.generated.h"

class AActor;

UINTERFACE(MinimalAPI)
class UWidgetHostAwareInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Notifies a widget about a host, usually from a widget component.
 */
class NINJACOMMONUI_API IWidgetHostAwareInterface
{
    
	GENERATED_BODY()

public:

	/** Provides the actor that owns the widget component. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NBS|Common UI|Interfaces|Widget Host Aware")
	AActor* GetWidgetHost() const;
	
	/** Sets the actor that owns the widget component. */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "NBS|Common UI|Interfaces|Widget Host Aware")
	void SetWidgetHost(AActor* Host);

};
