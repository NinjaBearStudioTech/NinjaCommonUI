// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "NinjaCommonActivatableWidget.generated.h"

UENUM(BlueprintType)
enum class ENinjaWidgetInputMode : uint8
{
	Default,
	GameAndMenu,
	Game,
	Menu
};

USTRUCT(BlueprintType)
struct NINJACOMMONUI_API FNinjaDefaultInputActionRow
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Binding", meta = (RowType = "/Script/CommonUI.CommonInputActionDataBase"))
	FDataTableRowHandle InputAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Default Binding")
	bool bDisplayInActionBar = true;
	
};

USTRUCT(BlueprintType)
struct NINJACOMMONUI_API FNinjaInputActionBindingHandle
{
	GENERATED_BODY()
	FUIActionBindingHandle Handle;
};

/**
 * Common Base Activatable Widget that simplifies input config, mouse capture and action setup.
 */
UCLASS(Abstract)
class NINJACOMMONUI_API UNinjaCommonActivatableWidget : public UCommonActivatableWidget
{

	DECLARE_DYNAMIC_DELEGATE_OneParam(FInputActionExecutedDelegate, FName, ActionName);
	
	GENERATED_BODY()

public:

	// -- Begin Widget implementation
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	// -- End Widget implementation

	// -- Begin ActivatableWidget implementation
	virtual TOptional<FUIInputConfig> GetDesiredInputConfig() const override;
	// -- End ActivatableWidget implementation
	
protected:

	/** The desired input mode to use while this UI is activated. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	ENinjaWidgetInputMode InputConfig = ENinjaWidgetInputMode::Default;

	/** The desired mouse behavior when the game gets input. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	EMouseCaptureMode GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;

	/** Determines if the mouse cursor should be displayed when this widget activates. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	bool bShowMouseCursor = true;
	
	/** All Input Rows that will be automatically registered. */
	UPROPERTY(EditDefaultsOnly, Category = Input)
	TArray<FNinjaDefaultInputActionRow> InputRows;

	/**
	 * Registers all default Input Rows set to this widget.
	 */
	UFUNCTION(BlueprintCallable, Category="NBS|Common UI|Widgets")
	void RegisterDefaultInputRows();
	
	/**
	 * Registers an Input Row and provides the handle for it.
	 */
	UFUNCTION(BlueprintCallable, Category="NBS|Common UI|Widgets")
	bool RegisterInputRow(const FNinjaDefaultInputActionRow& Binding, FNinjaInputActionBindingHandle& BindingHandle);

	/**
	 * Unregisters all bindings active on this widget.
	 */
	UFUNCTION(BlueprintCallable, Category="NBS|Common UI|Widgets")
	void UnregisterAllBindings();

	/**
	 * Unregisters the provided binding handle. 
	 */
	UFUNCTION(BlueprintCallable, Category="NBS|Common UI|Widgets")
	void UnregisterInputBinding(FNinjaInputActionBindingHandle BindingHandle);
	
	/**
	 * Handles an action that has been completed action.
	 * This includes default and hold actions that were completed.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "NBS|Common UI|Widgets")
	void HandleInputActionCompleted(FName ActionName);

	/**
	 * Handles a hold action that has started action.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "NBS|Common UI|Widgets")
	void HandleHoldInputActionStarted(FName ActionName);

	/**
	 * Handles the progress of a hold action.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "NBS|Common UI|Widgets")
	void HandleHoldInputActionProgressed(FName ActionName, float ElapsedTime);

	/**
	 * Handles a hold action that has been cancelled.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "NBS|Common UI|Widgets")
	void HandleHoldInputActionCancelled(FName ActionName);
	
private:

	/** All handles that are registered to this widget. */
	TArray<FUIActionBindingHandle> BindingHandles;
	
};
