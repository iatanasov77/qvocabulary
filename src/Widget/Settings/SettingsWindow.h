#ifndef WIDGET_SETTINGS_WINDOW_H
#define WIDGET_SETTINGS_WINDOW_H

#include <QWidget>
#include <QTreeWidgetItem>

namespace Ui {
	class SettingsWindow;
}

class SettingsWindow : public QWidget
{
    Q_OBJECT

	public:
		explicit SettingsWindow( QWidget *parent = 0 );
		~SettingsWindow();

	private:
		void initSettingsMenu();
		void showSettingsUnimpemented( QString settingsTitle );
		void showSettingsGeneral();

	    Ui::SettingsWindow* ui;

	private slots:
		void showSettings( QTreeWidgetItem* item, int column );
};

#endif // WIDGET_SETTINGS_WINDOW_H
