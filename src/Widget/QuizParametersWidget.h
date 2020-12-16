#ifndef WIDGET_QUIZPARAMETERS_H
#define WIDGET_QUIZPARAMETERS_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
	class QuizParametersWidget;
}

class QuizParametersWidget : public QWidget
{
    Q_OBJECT

	public:
		explicit QuizParametersWidget( QWidget *parent = 0 );
		~QuizParametersWidget();

		QPushButton* btnStartQuiz();
		void initGroups();

		int initDatabase();
		static const int NewDatabase	= 0;
		static const int OpenDatabase	= 1;

	private:
	    Ui::QuizParametersWidget* ui;
};

#endif // WIDGET_QUIZPARAMETERS_H
