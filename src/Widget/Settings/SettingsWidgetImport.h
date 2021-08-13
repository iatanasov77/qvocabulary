#ifndef WIDGET_SETTINGS_WIDGET_IMPORT_H
#define WIDGET_SETTINGS_WIDGET_IMPORT_H

#include <QWidget>
#include "ui_SettingsWidgetImport.h"

#include "AbstractSettingsWidget.h"

namespace Ui {
	class SettingsWidgetImport;
}

class SettingsWidgetImport : public AbstractSettingsWidget
{
    Q_OBJECT

	public:
		explicit SettingsWidgetImport( QWidget *parent = 0 );
		~SettingsWidgetImport();

		virtual void apply();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void init();

	    Ui::SettingsWidgetImport* ui;
	    QMap<QString, QVariant> importSettings;
};

#endif // WIDGET_SETTINGS_WIDGET_IMPORT_H
