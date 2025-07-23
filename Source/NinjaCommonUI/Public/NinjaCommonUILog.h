// Ninja Bear Studio Inc., all rights reserved.
#pragma once

#include "CoreMinimal.h"

NINJACOMMONUI_API DECLARE_LOG_CATEGORY_EXTERN(LogNinjaCommonUI, Log, All);

/**
 * Creates appropriate messages for the User Interface category.
 * 
 * @param Verbosity		Verbosity level of this message. See ELogVerbosity.
 * @param Message		Message string literal.
 */
#define CUI_LOG(Verbosity, Message) \
	UE_LOG(LogNinjaCommonUI, Verbosity, TEXT("%s: %s"), *(GetClass()->GetName() + "::" + __FUNCTION__), TEXT(Message));

/**
 * Creates appropriate messages for the User Interface category, with format arguments.
 * 
 * @param Verbosity		Verbosity level of this message. See ELogVerbosity.
 * @param Format		Format string literal in the style of printf.
 * @param Args			Comma-separated arguments used to format the message.
 */
#define CUI_LOG_ARGS(Verbosity, Format, ...) \
	UE_LOG(LogNinjaCommonUI, Verbosity, TEXT("%s: %s"), *(GetClass()->GetName() + "::" + __FUNCTION__), *FString::Printf(TEXT(Format), ##__VA_ARGS__));
