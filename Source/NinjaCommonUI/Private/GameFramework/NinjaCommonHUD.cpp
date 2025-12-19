// Ninja Bear Studio Inc., all rights reserved.
#include "GameFramework/NinjaCommonHUD.h"

#include "NinjaCommonUILog.h"
#include "NinjaCommonUITags.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/NinjaCommonGameplayWidget.h"

void ANinjaCommonHUD::BeginPlay()
{
	Super::BeginPlay();

	ShowGameplayWidget();
	
	APlayerController* MyPlayerController = GetOwningPlayerController();
	if (IsValid(MyPlayerController))
	{
		MyPlayerController->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::HandleNewPawn);
	}
}

void ANinjaCommonHUD::HandleNewPawn(APawn* NewPawn)
{
	if (IsValid(NewPawn))
	{
		ShowGameplayWidget();	
	}
}

void ANinjaCommonHUD::ShowGameplayWidget()
{
	APlayerController* PlayerController = GetOwningPlayerController();
	if (!IsValid(PlayerController))
	{
		CUI_LOG(Error, "Player Controller is unexpectedly missing!");
		return;
	}

	if (!IsValid(GameplayWidgetClass))
	{
		return;
	}

	if (IsValid(GameplayWidget))
	{
		GameplayWidget->DeactivateWidget();
		GameplayWidget->RemoveFromParent();
		GameplayWidget = nullptr;
	}	
	
	GameplayWidget = CreateWidget<UNinjaCommonGameplayWidget>(PlayerController, GameplayWidgetClass, TEXT("GameplayWidget"));
	CUI_LOG_ARGS(Log, "Created new Gameplay Widget %s.", *GetNameSafe(GameplayWidget));

	if (!IsValid(GameplayWidget))
	{
		CUI_LOG_ARGS(Error, "Unable to create the Gameplay Widget from class %s!", *GetNameSafe(GameplayWidgetClass));
		return;
	}

	GameplayWidget->AddToViewport();
	GameplayWidget->ActivateWidget();
}

void ANinjaCommonHUD::ToggleActivatableWidget(TObjectPtr<UCommonActivatableWidget>& Widget, const TFunction<UCommonActivatableWidget*()>& ActivationFunction)
{
	if (IsValid(Widget))
	{
		Widget->DeactivateWidget();
		Widget->RemoveFromParent();
		Widget = nullptr;
		return;
	}
	
	Widget = ActivationFunction();	
}

UCommonActivatableWidget* ANinjaCommonHUD::ShowInGameMenu()
{
	if (!IsValid(InGameMenuClass))
	{
		CUI_LOG(Warning, "In-Game Menu Widget is not set.");
		return nullptr;
	}

	return PushWidgetToStack(Tag_UI_Layer_Menu, InGameMenuClass);
}

UCommonActivatableWidget* ANinjaCommonHUD::ShowInventory()
{
	if (!IsValid(InventoryWidgetClass))
	{
		CUI_LOG(Warning, "Inventory Widget is not set.");
		return nullptr;
	}
	
	return PushWidgetToStack(Tag_UI_Layer_Menu, InventoryWidgetClass);
}

UCommonActivatableWidget* ANinjaCommonHUD::ShowStorage()
{
	if (!IsValid(StorageWidgetClass))
	{
		CUI_LOG(Warning, "Storage Widget is not set.");
		return nullptr;
	}
	
	return PushWidgetToStack(Tag_UI_Layer_Game, StorageWidgetClass);
}

UCommonActivatableWidget* ANinjaCommonHUD::ShowLoot()
{
	if (!IsValid(LootWidgetClass))
	{
		CUI_LOG(Warning, "Loot Widget is not set.");
		return nullptr;
	}
	
	return PushWidgetToStack(Tag_UI_Layer_Game, LootWidgetClass);
}

UCommonActivatableWidget* ANinjaCommonHUD::PushWidgetToStack(const FGameplayTag StackTag, const TSubclassOf<UCommonActivatableWidget>& WidgetClass)
{
	UNinjaCommonGameplayWidget* CurrentGameplayWidget = GetGameplayWidget();
	if (IsValid(CurrentGameplayWidget) && IsValid(WidgetClass))
	{
		return CurrentGameplayWidget->AddToStack(StackTag, WidgetClass);
	}

	return nullptr;
}

UNinjaCommonGameplayWidget* ANinjaCommonHUD::GetGameplayWidget() const
{
	return GameplayWidget;
}

void ANinjaCommonHUD::AddAsynchronousOperation(const FGameplayTag OperationTag)
{
	if (OperationTag.IsValid())
	{
		AsyncOperations.AddTag(OperationTag);
		static constexpr bool bOperationRunning = true;
		OnAsynchronousOperationNotificationStateChanged.Broadcast(AsyncOperations, bOperationRunning);
	}
}

void ANinjaCommonHUD::RemoveAsynchronousOperation(const FGameplayTag OperationTag)
{
	if (OperationTag.IsValid())
	{
		AsyncOperations.RemoveTag(OperationTag);
		const bool bOperationRunning = AsyncOperations.Num() > 0;
		OnAsynchronousOperationNotificationStateChanged.Broadcast(AsyncOperations, bOperationRunning);
	}
}