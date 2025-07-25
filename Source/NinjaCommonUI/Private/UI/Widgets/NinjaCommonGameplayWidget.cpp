// Ninja Bear Studio Inc., all rights reserved.
#include "UI/Widgets/NinjaCommonGameplayWidget.h"

#include "NinjaCommonUILog.h"
#include "NinjaCommonUITags.h"
#include "Widgets/CommonActivatableWidgetContainer.h"

UNinjaCommonGameplayWidget::UNinjaCommonGameplayWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bShowMouseCursor = false;
	InputConfig = ENinjaWidgetInputMode::Game;
	GameMouseCaptureMode = EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown;
}

UCommonActivatableWidget* UNinjaCommonGameplayWidget::AddToStack(const FGameplayTag StackTag, const TSubclassOf<UCommonActivatableWidget>& WidgetClass)
{
	UCommonActivatableWidgetStack* Stack = GetStack(StackTag);
	if (IsValid(Stack) && IsValid(WidgetClass))
	{
		UCommonActivatableWidget* Widget = Stack->AddWidget(WidgetClass);
		if (!Widget->IsActivated())
		{
			Widget->ActivateWidget();
		}
		
		OnWidgetAddedToStack(StackTag, Widget);
		return Widget;
	}

	return nullptr;	
}

void UNinjaCommonGameplayWidget::RemoveFromStack(const FGameplayTag StackTag, UCommonActivatableWidget* Widget)
{
	UCommonActivatableWidgetStack* Stack = GetStack(StackTag);
	if (IsValid(Stack) && IsValid(Widget))
	{
		Stack->RemoveWidget(*Widget);
		
		Widget->DeactivateWidget();
		Widget->RemoveFromParent();
		
		OnWidgetAddedToStack(StackTag, Widget);
	}
}

UCommonActivatableWidgetStack* UNinjaCommonGameplayWidget::GetStack_Implementation(const FGameplayTag StackTag) const
{
	if (StackTag == Tag_UI_Layer_Game)
	{
		return GameStack;
	}

	if (StackTag == Tag_UI_Layer_Menu)
	{
		return MenuStack;
	}

	CUI_LOG_ARGS(Warning, "No stack for tag %s.", *StackTag.ToString());
	return nullptr;
}
