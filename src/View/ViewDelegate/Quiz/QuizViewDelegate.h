#ifndef VIEW_QUIZVIEWDELEGATE_H
#define VIEW_QUIZVIEWDELEGATE_H

#include <QItemDelegate>

class QuizViewDelegate : public QItemDelegate
{
    Q_OBJECT

	public:
		QuizViewDelegate( QObject *parent = 0 );
		virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index );

};

#endif	// VIEW_QUIZVIEWDELEGATE_H
