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

		void setViewHeader( VocabularyMetaInfoPtr metaInfo );
		void loadGroup( int groupId );

	public slots:
		void insertWord();
		void onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );

	private:
		void initModel();

		QString randomWord();

		int currentGroup;
		Ui::QuizWidget* ui;

		//qx::QxModel<Quiz>* pModelQuiz;
		qx::QxModel<Vocabulary>* pModelVocabulary;

		qx::QxModel<QuizItem>* pModel;

		QList<int> hideColumns;
};

#endif // WIDGET_QUIZ_H
