// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "NinjaCommonWidgetComponent.generated.h"

/** 
 * A widget component that sets the owner to the widget and any ViewModel.
 */
UCLASS(ClassGroup=(NinjaCommonUI), meta=(BlueprintSpawnableComponent))
class NINJACOMMONUI_API UNinjaCommonWidgetComponent : public UWidgetComponent
{
	
	GENERATED_BODY()

public:

	UNinjaCommonWidgetComponent();

	// -- Begin Widget Component implementation
	virtual void InitWidget() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// -- End Widget Component implementation
	
protected:

	/**
	 * Time to wait before attempting initialization.
	 *
	 * It may take a few frames for the view models to initialize after the widget and the widget
	 * does not provide a delegate for that, so we'll need to try initializing again after a few
	 * frames.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget Host")
	float TimeBeforeReattemptInitialization;
	
	/**
	 * Max attempts to initialize the ViewModels, before giving up.
	 *
	 * This should only be necessary if you are not sure about the widgets that will be hosted by
	 * this component, but it is usually recommended that you only use this component with supported
	 * widgets/ViewModels.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widget host")
	int32 MaxInitializationAttempts;
	
	/**
	 * Tries to set the host to the widget.
	 */
	UFUNCTION()
	virtual void TrySetHostToWidget();

	/**
	 * Tries to set the host to the view models.
	 */	
	UFUNCTION()
	virtual void TrySetHostToViewModels();	
	
private:

	int32 InitializationAttempts;
	FTimerHandle InitializationHandle;	
	
};
