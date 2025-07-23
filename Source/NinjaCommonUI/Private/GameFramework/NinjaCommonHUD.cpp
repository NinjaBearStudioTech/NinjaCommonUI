// Ninja Bear Studio Inc., all rights reserved.
#include "GameFramework/NinjaCommonHUD.h"

#include "NinjaCommonUILog.h"
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

void ANinjaCommonHUD::ShowInventory()
{
	if (!IsValid(InventoryWidgetClass))
	{
		CUI_LOG(Warning, "Inventory Widget is not set.");
		return;
	}
	
	PushWidgetToStack(InventoryWidgetClass);
}

void ANinjaCommonHUD::PushWidgetToStack(const TSubclassOf<UCommonActivatableWidget>& WidgetClass)
{
	UNinjaCommonGameplayWidget* CurrentGameplayWidget = GetGameplayWidget();
	if (IsValid(CurrentGameplayWidget) && IsValid(WidgetClass))
	{
		CurrentGameplayWidget->AddToStack(WidgetClass);
	}
}

UNinjaCommonGameplayWidget* ANinjaCommonHUD::GetGameplayWidget() const
{
	return GameplayWidget;
}
