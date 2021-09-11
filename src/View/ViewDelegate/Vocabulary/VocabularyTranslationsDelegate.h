#ifndef VIEW_VOCABULARYTRANSLATIONS_DELEGATE_H
#define VIEW_VOCABULARYTRANSLATIONS_DELEGATE_H

#include <QStyledItemDelegate>

class VocabularyTranslationsDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		VocabularyTranslationsDelegate( QObject *parent = 0 );
		virtual void paint( QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
		bool editorEvent( QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index );
		QWidget* createEditor( QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index ) const;

	private:
		QRect textRect( QRect cellRect ) const;
		QRect buttonRect( QRect cellRect ) const;
		QStyleOptionButton createButton( QRect rect  ) const;
		bool isEmptyLine( const QModelIndex index ) const;

	signals:
		void buttonClicked( const QModelIndex &index );
};

#endif	// VIEW_VOCABULARYTRANSLATIONS_DELEGATE_H
