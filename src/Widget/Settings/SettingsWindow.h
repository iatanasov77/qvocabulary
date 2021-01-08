#ifndef WIDGET_SETTINGS_WINDOW_H
#define WIDGET_SETTINGS_WINDOW_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "AbstractSettingsWidget.h"

namespace Ui {
	class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

	public:
		explicit SettingsWindow( QWidget *parent = 0 );
		~SettingsWindow();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initSettingsMenu();
		void showSettingsUnimplemented( QString settingsTitle );
		void showSettingsGeneral();
		void showSettingsSpeaker();
		void setSettingsWidget( AbstractSettingsWidget* newWdg );

	    Ui::SettingsWindow* ui;
	    AbstractSettingsWidget* wdg;

	private slots:
		void showSettings( QTreeWidgetItem* item, int column );
		void applySettings();
		void saveAndExitSettings();
};

#endif // WIDGET_SETTINGS_WINDOW_H
