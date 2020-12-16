#ifndef WIDGET_QUIZ_WINDOW_H
#define WIDGET_QUIZ_WINDOW_H

#include "ui_HelpWindow.h"
#include <QWidget>

#include "Widget/QuizParametersWidget.h"
#include "Widget/QuizWidget.h"

namespace Ui {
	class QuizWindow;
}

class QuizWindow : public QWidget
{
    Q_OBJECT

	public:
		explicit QuizWindow( QWidget *parent = 0 );
		~QuizWindow();

	public slots:
		void startQuiz();
		void stopQuiz();

	private:
		void init();
		void initConnections();
		void initModels();

	    Ui::QuizWindow* ui;
	    QuizParametersWidget* wdgParameters;
	    QuizWidget* wdgQuiz;
};

#endif // WIDGET_QUIZ_WINDOW_H
