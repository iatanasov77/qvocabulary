#ifndef VIEW_VOCABULARYTRANSCRIPTIONSDELEGATE_H
#define VIEW_VOCABULARYTRANSCRIPTIONSDELEGATE_H

#include <QStyledItemDelegate>

class VocabularyTranscriptionsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		VocabularyTranscriptionsDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	private:
		QObject *view;

		QRect textRect( QRect cellRect ) const;
		QRect buttonRect( QRect cellRect ) const;
		QStyleOptionButton createButton( QRect rect  ) const;
		void setCursor( const QStyleOptionViewItem option, const QModelIndex index ) const;
		bool isEmptyLine( const QModelIndex index ) const;

	signals:
		void buttonClicked( const QModelIndex &index );
};

#endif	// VIEW_VOCABULARYTRANSCRIPTIONSDELEGATE_H
