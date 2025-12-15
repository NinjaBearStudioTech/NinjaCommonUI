// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/HUD.h"
#include "NinjaCommonHUD.generated.h"

class UNinjaCommonGameplayWidget;
class UCommonActivatableWidget;

/**
 * Base HUD that is integrated with the default Gameplay Widget type (UNinjaCommonGameplayWidget).
 * Supports common activatable windows such as Inventory, Loot, Vendors, Dialogue, Quests, Progression, etc.
 */
UCLASS(Abstract)
class NINJACOMMONUI_API ANinjaCommonHUD : public AHUD
{

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAsynchronousOperationNotificationStateChanged, FGameplayTagContainer, AsyncOperations, bool, bOperationRunning);
	
	GENERATED_BODY()

public:

	/** Notifies about asynchronous notifications that must be presented. */
	UPROPERTY(BlueprintAssignable)
	FAsynchronousOperationNotificationStateChanged OnAsynchronousOperationNotificationStateChanged;
	
	// -- Begin HUD implementation
	virtual void BeginPlay() override;
	// -- End HUD implementation	

	/**
	 * Registers an asynchronous operation, represented by its gameplay tag.
	 * This will most likely result in something like a "progress icon" to be shown in the UI.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	void AddAsynchronousOperation(FGameplayTag OperationTag);

	/**
	 * Removes an asynchronous operation, represented by its gameplay tag.
	 * If there are no operations left, then the UI should remove the "progress icon".
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	void RemoveAsynchronousOperation(FGameplayTag OperationTag);
	
	/**
	 * Shows the In-Game Menu (or "Pause Menu"), usually containing options such as settings, quit, etc.
	 * Usually the widget is responsible for deactivating itself.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	UCommonActivatableWidget* ShowInGameMenu();
	
	/**
	 * Shows the Inventory Window, instantiating one if necessary.
	 * Usually the widget is responsible for deactivating itself. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	UCommonActivatableWidget* ShowInventory();

	/**
	 * Shows the Storage Window, instantiating one if necessary.
	 * Usually the widget is responsible for deactivating itself. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	UCommonActivatableWidget* ShowStorage();
	
	/**
	 * Shows the Loot Window, instantiating one if necessary.
	 * Usually the widget is responsible for deactivating itself. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	UCommonActivatableWidget* ShowLoot();
	
	/**
	 * Provides the Gameplay Widget instantiated by this HUD. 
	 */
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	UNinjaCommonGameplayWidget* GetGameplayWidget() const;

	/**
	 * Pushes a widget to the UI stack, represented by the Gameplay Tag.
	 *
	 * @param StackTag		The stack representing the UI Layer where the widget will be pushed.
	 * @param WidgetClass	Widget class that will be pushed to the stack.
	 * @return				The widget instance that has been pushed to the stack.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	virtual UCommonActivatableWidget* PushWidgetToStack(UPARAM(meta = (categories = "UI.Layer")) FGameplayTag StackTag, const TSubclassOf<UCommonActivatableWidget>& WidgetClass);
	
protected:

	/** The main widget for the Gameplay. Will provide the activatable stack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UNinjaCommonGameplayWidget> GameplayWidgetClass;

	/** The widget for the in-game menu. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UCommonActivatableWidget> InGameMenuClass;
	
	/** The widget for the Inventory Window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UCommonActivatableWidget> InventoryWidgetClass;

	/** The widget for any external storage related to an Inventory. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UCommonActivatableWidget> StorageWidgetClass;
	
	/** The widget for the Loot Window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UCommonActivatableWidget> LootWidgetClass;
	
	/**
	 * The HUD must be prepared to initialize the pawn after Begin Play.
	 *
	 * This happens because it might take a moment for the Game Mode to organize and replicate
	 * the match, so during that time, players won't be ready to start. However, the HUD's
	 * Begin Play triggers with the Controller, and it won't have a pawn by then.
	 *
	 * By binding to the Pawn's delegates after the controller broadcasts the pawn is the safest
	 * way to ensure that we will have a pawn available when the binding needs to happen.
	 */
	UFUNCTION()
	virtual void HandleNewPawn(APawn* NewPawn);

	/**
	 * Shows the gameplay widget.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "NBS|Common UI|HUD")
	virtual void ShowGameplayWidget();

	/**
	 * Toggles the provided activatable widget provided to the function.
	 * The widget is most likely one of the internal widget instances.
	 */
	static void ToggleActivatableWidget(TObjectPtr<UCommonActivatableWidget>& Widget, const TFunction<UCommonActivatableWidget*()>& ActivationFunction);	
	
private:

	/** All async operations happening. */
	FGameplayTagContainer AsyncOperations = FGameplayTagContainer::EmptyContainer;
	
	/** Current Gameplay Widget used in the game. */
	UPROPERTY(Transient)
	TObjectPtr<UNinjaCommonGameplayWidget> GameplayWidget;	

};
