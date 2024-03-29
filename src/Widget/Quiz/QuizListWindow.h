#ifndef WIDGET_QUIZLIST_WINDOW_H
#define WIDGET_QUIZLIST_WINDOW_H

#include <QWidget>
#include <QTreeWidgetItem>
#include <QGroupBox>
#include <QResizeEvent>
#include <QToolButton>
#include <QSortFilterProxyModel>

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

		void displayItems( int quizId, QString quizTitle );
		void openQuiz( int quizId, QString quizTitle );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );
		virtual void resizeEvent( QResizeEvent* event );

	private:
		void initQuizList();
		QStringList quizItemHeaders();
		QSortFilterProxyModel* createItemsModel( int quizId );
		QGroupBox* quizButtons( QMap<QString, QVariant> properties );
		QToolButton* createToolButton( const QString &toolTip, const QIcon &icon, const char *member, QMap<QString, QVariant> properties );
		void initQuizListItem( QTreeWidgetItem* parent, int quizRow, QMap<QString, QVariant> properties );
		void initQuizListDetails( QTreeWidgetItem* parent, int quizRow );

		Ui::QuizListWindow *ui;
		qx::QxModel<Quiz> *pModel;
		QWidget *mw;

	private slots:
		void openQuiz();
		void deleteQuiz();
};

#endif // WIDGET_QUIZLIST_WINDOW_H
