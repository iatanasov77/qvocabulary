#ifndef ABSTRACT_SETTINGS_WIDGET_H
#define ABSTRACT_SETTINGS_WIDGET_H

#include <QWidget>

#include "Application/VsSettings.h"

class AbstractSettingsWidget : public QWidget
{
    Q_OBJECT

	public:
		explicit AbstractSettingsWidget( QString title, QWidget *parent = 0 );

		virtual QString title();
		virtual void apply() = 0;

	protected:
		QString _title;
		VsSettings* settings;
};

#endif // ABSTRACT_SETTINGS_WIDGET_H
