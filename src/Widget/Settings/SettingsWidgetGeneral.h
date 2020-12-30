#ifndef WIDGET_SETTINGS_WIDGET_GENERAL_H
#define WIDGET_SETTINGS_WIDGET_GENERAL_H

#include <QWidget>

namespace Ui {
	class SettingsWidgetGeneral;
}

class SettingsWidgetGeneral : public QWidget
{
    Q_OBJECT

	public:
		explicit SettingsWidgetGeneral( QWidget *parent = 0 );
		~SettingsWidgetGeneral();

	private:
		void init();

	    Ui::SettingsWidgetGeneral* ui;
};

#endif // WIDGET_SETTINGS_WIDGET_GENERAL_H
