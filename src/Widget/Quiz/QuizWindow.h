#ifndef WIDGET_QUIZ_WINDOW_H
#define WIDGET_QUIZ_WINDOW_H

#include <QWidget>
#include <QResizeEvent>

#include "Widget/Quiz/QuizParametersWidget.h"
#include "Widget/Quiz/QuizWidget.h"

#include "Entity/Quiz.h"

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

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );
		virtual void resizeEvent( QResizeEvent* event );

	private:
		void init();
		void initConnections();
		void initModels();

	    Ui::QuizWindow* ui;
	    QuizParametersWidget* wdgParameters;
	    QuizWidget* wdgQuiz;

};

#endif // WIDGET_QUIZ_WINDOW_H
