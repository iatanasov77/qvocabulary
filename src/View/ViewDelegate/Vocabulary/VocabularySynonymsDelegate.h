#ifndef VIEW_VOCABULARYSYNONYMSDELEGATE_H
#define VIEW_VOCABULARYSYNONYMSDELEGATE_H

#include "View/ViewDelegate/VocabularyViewDelegate.h"

class VocabularySynonymsDelegate : public VocabularyViewDelegate
{
    Q_OBJECT

	public:
		VocabularySynonymsDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	protected:
		QStyleOptionButton createButton( QRect rect  ) const;

	private:
		QObject *view;
		static int countWords;
		static QMap<int, QMap<int, QRect>> wordRects; // <int row, QMap words>

		QRect textRect( QRect cellRect, int wordNumber = 1 ) const;
		void createWidgets( QPainter *painter, QStyleOptionViewItem option, QModelIndex index ) const;
		QMap<int, QRect> createWidget(
			QPainter *painter,
			QStyleOptionViewItem option,
			QModelIndex index,
			QString word,
			int wordId,
			int wordNumber,
			QString wordTarget
		) const;

	signals:
		void synonymClicked( int wordId );
};

#endif	// VIEW_VOCABULARYSYNONYMSDELEGATE_H
