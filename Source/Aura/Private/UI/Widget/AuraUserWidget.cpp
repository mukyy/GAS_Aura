// Copyright Emircan Birbil


#include "UI/Widget/AuraUserWidget.h"

void UAuraUserWidget::SetWidgetController(UObject* InWidgetController)
{
	check(InWidgetController)
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
