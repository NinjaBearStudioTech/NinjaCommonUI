// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "NinjaCommonActivatableWidget.h"
#include "NinjaCommonGameplayWidget.generated.h"

class UCommonActivatableWidgetStack;

/**
 * Base gameplay widget that provides a stack that can be used to push widgets.
 * 
 * Requires a Stack Component (UCommonActivatableWidgetStack) named "Stack" on the widget,
 * for Widget Binding. This will be the stack used internally for adding/remove operations.
 */
UCLASS()
class NINJACOMMONUI_API UNinjaCommonGameplayWidget : public UNinjaCommonActivatableWidget
{
	
	GENERATED_BODY()
	
public:

	/**
	 * Adds a widget to the stack, automatically instantiating and activating it.
	 *
	 * @param WidgetClass			Type of widget that will be created and added to the stack.
	 * @return						The instance created and added to the stack. Can be used for removal.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|Widgets|Gameplay")
	virtual UCommonActivatableWidget* AddToStack(const TSubclassOf<UCommonActivatableWidget>& WidgetClass);

	/**
	 * Remove sa widget from the stack, deactivating it.
	 *
	 * @param Widget				Widget instance that will be removed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|Widgets|Gameplay")
	virtual void RemoveFromStack(UCommonActivatableWidget* Widget);

protected:
	
	/** Stack used to push overlays (death card, virtue cards, etc.) */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> Stack;

	/**
	 * Notifies that a widget has been added to the stack.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "Gameplay Widget")
	void OnWidgetAddedToStack(UCommonActivatableWidget* Widget);
	virtual void OnWidgetAddedToStack_Implementation(UCommonActivatableWidget* Widget) { }

	/**
	 * Notifies that a widget has been removed from the stack.
	 */	
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic,Category = "Gameplay Widget")
	void OnWidgetRemovedFromStack(UCommonActivatableWidget* Widget);
	virtual void OnWidgetRemovedFromStack_Implementation(UCommonActivatableWidget* Widget) { }
	
};
