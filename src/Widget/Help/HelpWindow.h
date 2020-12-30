#ifndef WIDGET_HELP_WINDOW_H
#define WIDGET_HELP_WINDOW_H

#include "ui_HelpWindow.h"
#include <QWidget>

class HelpWindow : public QWidget
{
    Q_OBJECT

	public:
		explicit HelpWindow( QWidget *parent = 0 );
		~HelpWindow();

	private:
		void initHelp();

	    Ui::HelpWindow *ui;
};

#endif // WIDGET_HELP_WINDOW_H
