#ifndef WIDGET_QUIZPARAMETERS_H
#define WIDGET_QUIZPARAMETERS_H

#include <QWidget>
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
		int time();

	public slots:
		void setDirection();
		void initTimer( bool on );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
	    Ui::QuizParametersWidget* ui;

	    QList<QCheckBox*> chkGroups;
	    EnumDirection direction;
};

#endif // WIDGET_QUIZPARAMETERS_H
