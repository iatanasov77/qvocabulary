#ifndef VIEW_VOCABULARY_DELEGATE_H
#define VIEW_VOCABULARY_DELEGATE_H

#include <QStyledItemDelegate>

class VocabularyViewDelegate : public QStyledItemDelegate
{
    Q_OBJECT

	public:
		VocabularyViewDelegate( QObject *parent = nullptr );

	protected:
		QObject *view;

		void setCursor( QEvent *event, const QStyleOptionViewItem option, const QModelIndex index ) const;
		void setCursor( const QStyleOptionViewItem option, const QModelIndex index ) const;
		bool isEmptyLine( const QModelIndex index ) const;

		QRect textRect( QRect cellRect ) const;
		QRect buttonRect( QRect cellRect ) const;
		virtual QStyleOptionButton createButton( QRect rect  ) const = 0; // pure virtual

	signals:
		void buttonClicked( const QModelIndex &index );
};

#endif	// VIEW_VOCABULARY_DELEGATE_H
