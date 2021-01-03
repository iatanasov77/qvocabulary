#ifndef WIDGET_QUIZLIST_WINDOW_H
#define WIDGET_QUIZLIST_WINDOW_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QGroupBox>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/Quiz.h"

namespace Ui {
	class QuizListWindow;
}

class QuizListWindow : public QWidget
{
    Q_OBJECT

	public:
		explicit QuizListWindow( QWidget *parent = 0 );
		~QuizListWindow();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initQuizList();
		QGroupBox* quizButtons( int quizId );
		void initQuizListProperties( QTreeWidgetItem* parent, int quizRow );

		Ui::QuizListWindow* ui;
		QList<int> hideItemColumns;
		qx::QxModel<Quiz>* pModel;

	private slots:
		void displayItems();
		void deleteQuiz();
};

#endif // WIDGET_QUIZLIST_WINDOW_H
