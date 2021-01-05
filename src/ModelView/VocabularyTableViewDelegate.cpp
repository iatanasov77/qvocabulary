#include "VocabularyTableViewDelegate.h"

VocabularyTableViewDelegate::VocabularyTableViewDelegate( QObject *parent ) : QStyledItemDelegate( parent )
{

}

void VocabularyTableViewDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const
{
	QStyleOptionViewItem op( option );

	if ( index.column() == 2 ) {
		op.font.setBold( true );
//		op.palette.setColor( QPalette::Normal, QPalette::Background, Qt::black );
//		op.palette.setColor( QPalette::Normal, QPalette::Foreground, Qt::white );
	}

	QStyledItemDelegate::paint( painter, op, index );
}
