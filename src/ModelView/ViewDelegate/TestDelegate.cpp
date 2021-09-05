#include "TestDelegate.h"

#include <QApplication>

int TestDelegate::countWords;

TestDelegate::TestDelegate( QObject *parent ) : QStyledItemDelegate( parent )
{
}

void TestDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const {
	if ( ! index.isValid() )
		QStyledItemDelegate::paint( painter, option, QModelIndex() );	// draw the selection background only


	if ( index.column() == 6 ) {
		painter->save();

		QString text				= index.data( Qt::DisplayRole ).toString();
		QStringList words			= text.split( "," );
		TestDelegate::countWords	= words.size();
		QStyleOptionViewItem op( option );

		int widgetNumber	= 1;
		foreach( QString word, words ) {
			word	= word.trimmed();

			if ( word.size( ) ) {
				op.rect	= widgetRect( option.rect, widgetNumber );
				initStyleOption( &op, index );
				createWidget( painter, op, index, word );
				widgetNumber++;
			}
		}

		painter->restore();
	}
}

QRect TestDelegate::widgetRect( QRect cellRect, int widgetNumber ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x 		= cellRect.left();
	int y 		= cellRect.top();
	int w 		= cellRect.width() - btnSize;
	if ( TestDelegate::countWords ) {
		w	= w / TestDelegate::countWords;
		if ( widgetNumber > 1 )
			x	= x + ( w * ( widgetNumber - 1 ) );
	}
	int h 		= cellRect.height();

	return QRect( x, y, w, h );
}

void TestDelegate::createWidget( QPainter *painter, QStyleOptionViewItem op, QModelIndex index, QString text ) const
{
	Q_UNUSED( index );

	op.icon 	= QIcon( ":/Resources/icons/dictionary.svg" );
	op.text		= text;
	op.font.setBold( true );
	op.font.setUnderline( true );

	QStyle *style	= op.widget ? op.widget->style() : QApplication::style();
	style->drawControl( QStyle::CE_ItemViewItem, &op, painter, op.widget );
}
