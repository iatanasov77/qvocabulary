#include "AbstractSettingsWidget.h"

#include "Application/VsSettings.h"

AbstractSettingsWidget::AbstractSettingsWidget( QString title, QWidget *parent ) : QWidget( parent )
{
	_title		= title;
	settings	= VsSettings::instance();
}

QString AbstractSettingsWidget::title()
{
	return _title;
}
