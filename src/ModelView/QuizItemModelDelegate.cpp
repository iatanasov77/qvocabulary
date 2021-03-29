#include "QuizItemModelDelegate.h"

#include <QPainter>
#include <QApplication>
#include <QIcon>
#include <QDebug>

QuizItemModelDelegate::QuizItemModelDelegate( QObject *parent, bool displayAnswerStatus ) : QStyledItemDelegate( parent )
{
	_displayAnswerStatus	= displayAnswerStatus;
	_iconSize 				= 16;
	_rightPadding			= 16;
}

void QuizItemModelDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const {
	QIcon icon;
	QStyleOptionViewItem op( option );
	initStyleOption( &op, index );

	if ( index.column() == 4 && _displayAnswerStatus ) {
		//QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
		if ( index.siblingAtColumn( 5 ).data().toBool() ) {
			icon	= QIcon( ":/Resources/icons/Symbol_OK.svg" );
		} else {
			icon	= QIcon( ":/Resources/icons/close.svg" );
		}

		QRect iconRect = _iconRect( option.rect );
		if ( ! icon.isNull() ) {
			painter->drawPixmap( iconRect.center(), icon.pixmap( _iconSize, _iconSize ) );
		}
	}

	op.rect	= _textRect( option.rect );
	QStyledItemDelegate::paint( painter, op, index );
}

QRect QuizItemModelDelegate::_textRect( QRect cellRect ) const
{
	int x = cellRect.left();
	int y = cellRect.top();
	int w = cellRect.width() - _iconSize - _rightPadding;
	int h = cellRect.height();

	return QRect( x, y, w, h );
}

QRect QuizItemModelDelegate::_iconRect( QRect cellRect ) const
{
	int x = cellRect.left() + cellRect.width() - _iconSize - _rightPadding;
	int y = cellRect.top();
	int w = _iconSize;
	int h = _iconSize;

	return QRect( x, y, w, h );
}
