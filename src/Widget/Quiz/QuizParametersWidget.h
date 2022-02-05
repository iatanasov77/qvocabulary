#ifndef WIDGET_QUIZPARAMETERS_H
#define WIDGET_QUIZPARAMETERS_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>

#include "GlobalTypes.h"
#include "Entity/VocabularyMetaInfo.h"

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
		void setMetaInfo( VocabularyMetaInfoPtr metaInfo );
		void initGroups();

		QList<QCheckBox*> getChkGroups();
		EnumDirection getDirection();
		QCheckBox* getChkRandomize();
		QCheckBox* getChkDisplayTranscriptions();
		int time();
		int wordsCount();
		QString wordsFrom();

	public slots:
		void setGroups();
		void setDirection();
		void initTimer( bool on );
		void initWordCount( bool on );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
	    Ui::QuizParametersWidget* ui;

	    QList<QCheckBox*> chkGroups;
	    EnumDirection direction;
	    QMap<QString, QVariant> quizSettings;

	    void clearGroups( QLayout *box );
	    void setVocabularyGroups( QLayout *box );
	    void setArchiveGroups( QLayout *box );

	    void initDirection();
};

#endif // WIDGET_QUIZPARAMETERS_H
