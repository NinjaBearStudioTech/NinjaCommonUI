// Ninja Bear Studio Inc., all rights reserved.
#include "UI/Widgets/NinjaCommonActivatableWidget.h"

#include "Input/CommonUIInputTypes.h"

void UNinjaCommonActivatableWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RegisterDefaultInputRows();
}

void UNinjaCommonActivatableWidget::NativeDestruct()
{
	UnregisterAllBindings();
	Super::NativeDestruct();
}

TOptional<FUIInputConfig> UNinjaCommonActivatableWidget::GetDesiredInputConfig() const
{
	// Common UI. Sigh... >.<
	const bool bHideCursorDuringViewportCapture = !bShowMouseCursor;
	
	switch (InputConfig)
	{
	case ENinjaWidgetInputMode::GameAndMenu:
		return FUIInputConfig(ECommonInputMode::All, GameMouseCaptureMode, bHideCursorDuringViewportCapture);
	case ENinjaWidgetInputMode::Game:
		return FUIInputConfig(ECommonInputMode::Game, GameMouseCaptureMode, bHideCursorDuringViewportCapture);
	case ENinjaWidgetInputMode::Menu:
		return FUIInputConfig(ECommonInputMode::Menu, EMouseCaptureMode::NoCapture, bHideCursorDuringViewportCapture);
	case ENinjaWidgetInputMode::Default:
	default:
		return TOptional<FUIInputConfig>();
	}
}

void UNinjaCommonActivatableWidget::RegisterDefaultInputRows()
{
	for (const FNinjaDefaultInputActionRow& Binding : InputRows)
	{
		FNinjaInputActionBindingHandle BindingHandle;
		RegisterInputRow(Binding, BindingHandle);
	}
}

bool UNinjaCommonActivatableWidget::RegisterInputRow(const FNinjaDefaultInputActionRow& Binding, FNinjaInputActionBindingHandle& BindingHandle)
{
	if (Binding.InputAction.IsNull())
	{
		return false;
	}

	const FDataTableRowHandle& InputAction = Binding.InputAction;
		
	FInputActionExecutedDelegate Callback;
	Callback.BindDynamic(this, &ThisClass::HandleInputActionCompleted);

	FBindUIActionArgs BindArgs(InputAction, FSimpleDelegate::CreateLambda([InputAction, Callback]()
	{
		// ReSharper disable once CppExpressionWithoutSideEffects
		Callback.ExecuteIfBound(InputAction.RowName);
	}));

	BindArgs.bDisplayInActionBar = Binding.bDisplayInActionBar;
	if (BindArgs.ActionHasHoldMappings())
	{
		BindArgs.OnHoldActionPressed.BindLambda([&, InputAction]()
		{
			HandleHoldInputActionStarted(InputAction.RowName);
		});

		BindArgs.OnHoldActionProgressed.BindLambda([&, InputAction](const float ElapsedTime)
		{
			HandleHoldInputActionProgressed(InputAction.RowName, ElapsedTime);
		});

		BindArgs.OnHoldActionReleased.BindLambda([&, InputAction]()
		{
			HandleHoldInputActionCancelled(InputAction.RowName);
		});				
	}
	
	BindingHandle.Handle = RegisterUIActionBinding(BindArgs);
	BindingHandles.Add(BindingHandle.Handle);
	return true;
}

void UNinjaCommonActivatableWidget::UnregisterAllBindings()
{
	for (FUIActionBindingHandle Handle : BindingHandles)
	{
		if (Handle.IsValid())
		{
			Handle.Unregister();
		}
	}
	BindingHandles.Empty();
}

void UNinjaCommonActivatableWidget::UnregisterInputBinding(FNinjaInputActionBindingHandle BindingHandle)
{
	if (BindingHandle.Handle.IsValid())
	{
		BindingHandle.Handle.Unregister();
		BindingHandles.Remove(BindingHandle.Handle);
	}
}

void UNinjaCommonActivatableWidget::HandleHoldInputActionStarted_Implementation(FName ActionName)
{
	// Handled by concrete widgets.
}

void UNinjaCommonActivatableWidget::HandleHoldInputActionProgressed_Implementation(FName ActionName, float ElapsedTime)
{
	// Handled by concrete widgets.
}

void UNinjaCommonActivatableWidget::HandleInputActionCompleted_Implementation(FName ActionName)
{
	// Handled by concrete widgets.
}

void UNinjaCommonActivatableWidget::HandleHoldInputActionCancelled_Implementation(FName ActionName)
{
	// Handled by concrete widgets.
}
