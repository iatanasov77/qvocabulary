#ifndef MODELDELEGATE_QUIZITEMMODELDELEGATE_H
#define MODELDELEGATE_QUIZITEMMODELDELEGATE_H

#include <QStyledItemDelegate>

class QuizItemModelDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		QuizItemModelDelegate( QObject *parent = 0, int statusColumn = 5, bool displayAnswerStatus = false, bool displayAnswerText = true );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

	protected:
		virtual void initStyleOption( QStyleOptionViewItem *option, const QModelIndex &index ) const;

	private:
		int _statusColumn;
		bool _displayAnswerStatus;
		bool _displayAnswerText;
		int _iconSize;
		int _rightPadding;

		QRect _textRect( QRect cellRect ) const;
		QRect _iconRect( QRect cellRect ) const;
};

#endif	// MODELDELEGATE_QUIZITEMMODELDELEGATE_H
