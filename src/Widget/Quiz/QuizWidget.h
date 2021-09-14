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
		QPushButton* btnOpenQuiz();

		void setQuiz( int quizId, QMap<QString, QVariant> parameters );
		void startQuiz();
		void finishQuiz();

	signals:
		void quizFinished();

	public slots:
		void insertWord();
		void onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
		void updateTimer();
		void modelRowsInserted( const QModelIndex & parent, int start, int end );
		void openQuiz();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initTimer( int time = 0 );
		void initView();
		void initModel();
		void insertWordHelpData( int quizItemRow, int vocabularyRow );

		Ui::QuizWidget* ui;
		QWidget *qw;
		QList<int> hideColumns;

		VocabularyMetaInfoPtr metaInfo;
		QuizPtr quiz;

		qx::QxModel<QuizItem> *pModel;
		qx::IxModel *pModelVocabulary;
		QVector<int> itemsRange;
		int questionsNumber;
		int rightAnswers;
		int timerSeconds;
};

#endif // WIDGET_QUIZ_H
