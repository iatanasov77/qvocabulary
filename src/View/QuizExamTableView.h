#ifndef VIEW_QUIZEXAMTABLEVIEW_H
#define VIEW_QUIZEXAMTABLEVIEW_H

#include <QTableView>

#include <QEvent>
#include <QDialog>
#include <QLabel>

class QuizExamTableView : public QTableView
{
	Q_OBJECT

	public:
		QuizExamTableView( QWidget *parent = Q_NULLPTR );
		bool eventFilter( QObject *watched, QEvent *event );

	private:
		QDialog *popup;
		QLabel *popupLabel;

		void initHeader();
		void initPopup();
		void showPopup ( const QModelIndex &index ) const;
		QString popupText( const QModelIndex &index  ) const;
};

#endif	// VIEW_QUIZEXAMTABLEVIEW_H
