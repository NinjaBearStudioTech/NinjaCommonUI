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
	
	GENERATED_BODY()

public:

	// -- Begin HUD implementation
	virtual void BeginPlay() override;
	// -- End HUD implementation	

	/**
	 * Shows the Inventory Window, instantiating one if necessary.
	 * Usually the widget is responsible for deactivating itself. 
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|HUD")
	void ShowInventory();

	/**
	 * Provides the Gameplay Widget instantiated by this HUD. 
	 */
	UFUNCTION(BlueprintPure, BlueprintCosmetic, Category = "Ninja Common UI|HUD")
	UNinjaCommonGameplayWidget* GetGameplayWidget() const;

protected:

	/** The main widget for the Gameplay. Will provide the activatable stack. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UNinjaCommonGameplayWidget> GameplayWidgetClass;
	
	/** A widget representing the Inventory Window. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Widgets")
	TSubclassOf<UCommonActivatableWidget> InventoryWidgetClass;

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
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|HUD")
	virtual void ShowGameplayWidget();

	/**
	 * Pushes a widget to the UI stack, represented by the Gameplay Tag.
	 */
	UFUNCTION(BlueprintCallable, BlueprintCosmetic, Category = "Ninja Common UI|HUD")
	virtual void PushWidgetToStack(UPARAM(meta = (categories = "UI.Layer")) FGameplayTag StackTag, const TSubclassOf<UCommonActivatableWidget>& WidgetClass);
	
private:

	/** Current Gameplay Widget used in the game. */
	UPROPERTY(Transient)
	TObjectPtr<UNinjaCommonGameplayWidget> GameplayWidget;	

};
