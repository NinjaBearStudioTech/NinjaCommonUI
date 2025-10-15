// Ninja Bear Studio Inc., all rights reserved.
#include "GameFramework/NinjaCommonHUD.h"

#include "NinjaCommonUILog.h"
#include "NinjaCommonUITags.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/NinjaCommonGameplayWidget.h"

void ANinjaCommonHUD::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* MyPlayerController = GetOwningPlayerController();
	if (IsValid(MyPlayerController))
	{
		APawn* MyPawn = MyPlayerController->GetPawn();
		if (IsValid(MyPawn))
		{
			HandleNewPawn(MyPawn);
		}

		// Still, we should get notified if the pawn ever changes.
		MyPlayerController->GetOnNewPawnNotifier().AddUObject(this, &ThisClass::HandleNewPawn);
	}	
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

void ANinjaCommonHUD::HandleNewPawn(APawn* NewPawn)
{
	if (!IsValid(NewPawn))
	{
		return;
	}

	ShowGameplayWidget();
}

void ANinjaCommonHUD::ShowGameplayWidget()
{
	if (IsValid(GameplayWidgetClass))
	{
		// Make sure that we'll create a Gameplay Widget if we don't have one yet!
		if (!IsValid(GameplayWidget))
		{
			APlayerController* MyPlayerController = GetOwningPlayerController();
			GameplayWidget = CreateWidget<UNinjaCommonGameplayWidget>(MyPlayerController, GameplayWidgetClass, "GameplayWidget");
			CUI_LOG_ARGS(Log, "Created new Gameplay Widget %s.", *GetNameSafe(GameplayWidget));
		}

		if (ensureMsgf(IsValid(GameplayWidget), TEXT("Expected a Gameplay Widget Instance!")))
		{
			if (!GameplayWidget->IsInViewport())
			{
				CUI_LOG(Verbose, "Adding Gameplay Widget to viewport.");
				GameplayWidget->AddToViewport();	
			}

			if (!GameplayWidget->IsActivated())
			{
				CUI_LOG(Verbose, "Activating Gameplay Widget.");
				GameplayWidget->ActivateWidget();	
			}	
		}
	}
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
