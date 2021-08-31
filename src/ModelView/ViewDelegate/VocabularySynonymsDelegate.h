#ifndef VIEW_VOCABULARYSYNONYMSDELEGATE_H
#define VIEW_VOCABULARYSYNONYMSDELEGATE_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QTextDocument>

class VocabularySynonymsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		VocabularySynonymsDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	private:
		QObject *view;
		static QMap<int, QMap<int, QRect>> wordRects; // <int row, QMap words>

		QRect textRect( QRect cellRect, int wordNumber = 1 ) const;
		QRect buttonRect( QRect cellRect ) const;
		void createWord( QPainter *painter, QStyleOptionViewItem op, QModelIndex index, QString text, int wordId ) const;
		QStyleOptionButton createButton( QRect rect  ) const;
		bool isEmptyLine( const QModelIndex index ) const;

	signals:
		void buttonClicked( const QModelIndex &index );
		void synonymClicked( int wordId );
};

#endif	// VIEW_VOCABULARYSYNONYMSDELEGATE_H
