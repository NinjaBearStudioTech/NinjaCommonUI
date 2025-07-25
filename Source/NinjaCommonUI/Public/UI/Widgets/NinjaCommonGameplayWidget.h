// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
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

	UNinjaCommonGameplayWidget(const FObjectInitializer& ObjectInitializer);
	
	/**
	 * Adds a widget to the stack, automatically instantiating and activating it.
	 *
	 * @param StackTag				Gameplay Tag representing the desired stack.
	 * @param WidgetClass			Type of widget that will be created and added to the stack.
	 * @return						The instance created and added to the stack. Can be used for removal.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|Widgets|Gameplay")
	virtual UCommonActivatableWidget* AddToStack(UPARAM(meta = (categories = "UI.Layer")) FGameplayTag StackTag, const TSubclassOf<UCommonActivatableWidget>& WidgetClass);

	/**
	 * Remove sa widget from the stack, deactivating it.
	 *
	 * @param StackTag				Gameplay Tag representing the desired stack.
	 * @param Widget				Widget instance that will be removed.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|Widgets|Gameplay")
	virtual void RemoveFromStack(UPARAM(meta = (categories = "UI.Layer")) FGameplayTag StackTag, UCommonActivatableWidget* Widget);

protected:
	
	/**
	 * Stack used to push widgets relative to the primary gameplay.
	 * This is where you'd add gameplay elements like HUD parts, quick slots, etc.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> GameStack;

	/**
	 * Stack used to push menus that will be on top of any gameplay UI.
	 * This is where you'd add non-gameplay-related windows like pause or settings menu.
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UCommonActivatableWidgetStack> MenuStack;

	/**
	 * Provides a stack related to a Gameplay Tag.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "Gameplay Widget", meta=(ForceAsFunction))
	UCommonActivatableWidgetStack* GetStack(UPARAM(meta = (categories = "UI.Layer")) FGameplayTag StackTag) const;
	
	/**
	 * Notifies that a widget has been added to the stack.
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic, Category = "Gameplay Widget")
	void OnWidgetAddedToStack(FGameplayTag StackTag, UCommonActivatableWidget* Widget);
	virtual void OnWidgetAddedToStack_Implementation(FGameplayTag StackTag, UCommonActivatableWidget* Widget) { }

	/**
	 * Notifies that a widget has been removed from the stack.
	 */	
	UFUNCTION(BlueprintNativeEvent, BlueprintCosmetic,Category = "Gameplay Widget")
	void OnWidgetRemovedFromStack(FGameplayTag StackTag, UCommonActivatableWidget* Widget);
	virtual void OnWidgetRemovedFromStack_Implementation(FGameplayTag StackTag, UCommonActivatableWidget* Widget) { }
	
};
