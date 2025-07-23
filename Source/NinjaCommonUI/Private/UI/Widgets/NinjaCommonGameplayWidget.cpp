// Ninja Bear Studio Inc., all rights reserved.
#include "UI/Widgets/NinjaCommonGameplayWidget.h"

#include "Widgets/CommonActivatableWidgetContainer.h"

UCommonActivatableWidget* UNinjaCommonGameplayWidget::AddToStack(const TSubclassOf<UCommonActivatableWidget>& WidgetClass)
{
	if (IsValid(Stack) && IsValid(WidgetClass))
	{
		UCommonActivatableWidget* Widget = Stack->AddWidget(WidgetClass);
		if (!Widget->IsActivated())
		{
			Widget->ActivateWidget();
		}

		OnWidgetAddedToStack(Widget);
		return Widget;
	}

	return nullptr;	
}

void UNinjaCommonGameplayWidget::RemoveFromStack(UCommonActivatableWidget* Widget)
{
	if (IsValid(Stack) && IsValid(Widget))
	{
		Widget->DeactivateWidget();
		Widget->RemoveFromParent();
		OnWidgetAddedToStack(Widget);
	}	
}
