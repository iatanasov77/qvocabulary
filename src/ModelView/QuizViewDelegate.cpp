#include "QuizViewDelegate.h"

QuizViewDelegate::QuizViewDelegate( QObject *parent ) :
    QItemDelegate( parent )
{

}

QWidget* QuizViewDelegate::createEditor(
	QWidget *parent,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
	return QItemDelegate::createEditor( parent, option, index );
/*
	if ( ! index.isValid() )
		return;

	QRect rect = option.rect.adjusted( 1, 1, -1, -1 );

	QRect buttonRect( rect );
	buttonRect.setHeight( 30 );

	QStyleOptionButton button;
	button.rect 	= buttonRect;
	button.text 	= index.siblingAtColumn( 1 ).data().toString();

	if( _currRow == index.row() && _event != 1 )
		button.state	= QStyle::State_Sunken | QStyle::State_Enabled;
	else
		button.state	= QStyle::State_Raised | QStyle::State_Enabled;

	QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter );
*/
}
