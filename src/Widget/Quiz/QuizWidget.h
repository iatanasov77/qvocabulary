#ifndef WIDGET_QUIZ_H
#define WIDGET_QUIZ_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>
#include <QPushButton>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Entity/Quiz.h"
#include "Entity/QuizItem.h"

namespace Ui {
	class QuizWidget;
}

class QuizWidget : public QWidget
{
    Q_OBJECT

	public:
		explicit QuizWidget( QWidget *parent = 0 );
		~QuizWidget();

		QPushButton* btnStopQuiz();

		void setQuiz( int quizId, QList<QString> groups, bool randomize = false, int time = 0 );
		void finishQuiz();

	signals:
		void quizFinished();

	public slots:
		void insertWord();
		void onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
		void updateTimer();
		void modelRowsInserted( const QModelIndex & parent, int start, int end );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initTimer( int time = 0 );
		void initModel();

		Ui::QuizWidget* ui;
		QList<int> hideColumns;

		VocabularyMetaInfoPtr metaInfo;
		QuizPtr quiz;

		qx::QxModel<QuizItem>* pModel;
		qx::QxModel<Vocabulary>* pModelVocabulary;
		QVector<int> itemsRange;
		int rightAnswers;
		int timerSeconds;

};

#endif // WIDGET_QUIZ_H
