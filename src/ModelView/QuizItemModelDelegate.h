#ifndef MODELDELEGATE_QUIZITEMMODELDELEGATE_H
#define MODELDELEGATE_QUIZITEMMODELDELEGATE_H

#include <QStyledItemDelegate>

class QuizItemModelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		QuizItemModelDelegate( QObject *parent = 0, bool displayAnswerStatus = false );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	private:
		bool _displayAnswerStatus;
		int _iconSize;
		int _rightPadding;

		QRect _textRect( QRect cellRect ) const;
		QRect _iconRect( QRect cellRect ) const;
};

#endif	// MODELDELEGATE_QUIZITEMMODELDELEGATE_H
