// Ninja Bear Studio Inc., all rights reserved.
#include "Components/NinjaCommonWidgetComponent.h"

#include "MVVMSubsystem.h"
#include "NinjaCommonUILog.h"
#include "Interfaces/WidgetHostAwareInterface.h"
#include "View/MVVMView.h"

class UMVVMView;

UNinjaCommonWidgetComponent::UNinjaCommonWidgetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	TimeBeforeReattemptInitialization = 0.2f;
	MaxInitializationAttempts = 10;
	InitializationAttempts = 0;	
}

void UNinjaCommonWidgetComponent::InitWidget()
{
	Super::InitWidget();
	
	TrySetHostToWidget();
	TrySetHostToViewModels();
}

void UNinjaCommonWidgetComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	GetWorld()->GetTimerManager().ClearTimer(InitializationHandle);
	Super::EndPlay(EndPlayReason);
}

void UNinjaCommonWidgetComponent::TrySetHostToWidget()
{
	AActor* Host = GetOwner();
	if (!IsValid(Host))
	{
		return;
	}

	UUserWidget* MyWidget = GetWidget();
	if (!IsValid(MyWidget))
	{
		return;
	}

	if (MyWidget->Implements<UWidgetHostAwareInterface>())
	{
		IWidgetHostAwareInterface::Execute_SetWidgetHost(MyWidget, Host);
	}
}

void UNinjaCommonWidgetComponent::TrySetHostToViewModels()
{
	AActor* Host = GetOwner();
	if (!IsValid(Host))
	{
		return;
	}

    const UUserWidget* MyWidget = GetWidget();
    if (!IsValid(MyWidget))
    {
        CUI_LOG_ARGS(Warning, "Widget is invalid.");
        GetWorld()->GetTimerManager().ClearTimer(InitializationHandle);
        return;
    }

    const UMVVMView* View = UMVVMSubsystem::GetViewFromUserWidget(MyWidget);
    if (!IsValid(View))
    {
        CUI_LOG_ARGS(Warning, "MVVM View is invalid.");
        GetWorld()->GetTimerManager().ClearTimer(InitializationHandle);
        return;
    }

    FTimerManager& TimerManager = GetWorld()->GetTimerManager();

    if (View->AreSourcesInitialized())
    {
        TimerManager.ClearTimer(InitializationHandle);

        const TArrayView<const FMVVMView_Source> Sources = View->GetSources();
        if (Sources.Num() == 0)
        {
        	CUI_LOG_ARGS(Warning, "View is initialized but Sources are empty. Widget: %s", *GetNameSafe(MyWidget));
            return;
        }

        bool bAnySourceSet = false;
        for (const FMVVMView_Source& ViewSource : Sources)
        {
            UObject* Source = ViewSource.Source;
            if (IsValid(Source) && Source->Implements<UWidgetHostAwareInterface>())
            {
                IWidgetHostAwareInterface::Execute_SetWidgetHost(Source, Host);
            	CUI_LOG_ARGS(Verbose, "SetInteractionTarget SUCCESS: Source = %s, Host = %s", *GetNameSafe(Source), *GetNameSafe(Host));
                bAnySourceSet = true;
            }
        }

        if (!bAnySourceSet)
        {
        	CUI_LOG_ARGS(Warning, "No valid sources found that implement WidgetHostAwareInterface.");
        }
    }
    else
    {
        // Max attempts check
        InitializationAttempts++;
        if (InitializationAttempts >= MaxInitializationAttempts)
        {
        	CUI_LOG_ARGS(Warning, "Max initialization attempts reached (%d). Stopping retries.", MaxInitializationAttempts);
            TimerManager.ClearTimer(InitializationHandle);
        }
        else
        {
            if (!TimerManager.IsTimerActive(InitializationHandle))
            {
                TimerManager.SetTimer(InitializationHandle, this, &ThisClass::TrySetHostToViewModels, TimeBeforeReattemptInitialization, true, 0.f);
                CUI_LOG_ARGS(Log, "Sources not yet initialized. Retrying (%d/%d)...", InitializationAttempts, MaxInitializationAttempts);
            }
        }
    }	
}
