#include "SideBarListViewDelegate.h"

#include <QApplication>
#include <QtGui>
#include <QSize>
#include <QListView>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "Entity/Vocabulary.h"
#include "Entity/ArchiveWord.h"

SideBarListViewDelegate::SideBarListViewDelegate( int currRow, bool inArchive, QObject *parent ) :
    QItemDelegate( parent )
{
	_inArchive	= inArchive;
	_currRow	= currRow;
	_event		= 0;
}

void SideBarListViewDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const
{
	if ( ! index.isValid() )
		return;

	QStyleOptionButton button;
	button.rect 	= option.rect;

	// Set Button Text
	QString query	= QString( "WHERE group_id=%1" ).arg( index.siblingAtColumn( 0 ).data().toInt() );
	long wordsCount	= 0;
	if ( _inArchive ) {
		wordsCount	= qx::dao::count<ArchiveWord>( qx::QxSqlQuery( query ) );
	} else {
		wordsCount	= qx::dao::count<Vocabulary>( qx::QxSqlQuery( query ) );
	}
	button.text 	= QString( "%1 (%2)" ).arg( index.siblingAtColumn( 1 ).data().toString() ).arg( wordsCount );

	// Set Button State
	if( _currRow == index.row() && _event != 1 )
		button.state	= QStyle::State_Sunken | QStyle::State_Enabled;
	else
		button.state	= QStyle::State_Raised | QStyle::State_Enabled;

	QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter );
}

QSize SideBarListViewDelegate::sizeHint( const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	QSize result = QItemDelegate::sizeHint( option, index );

	// Different Size Policy at Different OS
	#if ( defined (_WIN32) || defined (_WIN64) )
		//result.setWidth( 1 );
		result.setHeight( result.height() * 1.8 );
    #elif ( defined (LINUX) || defined (__linux__) )
		//result.setWidth( 1 );
		result.setHeight( result.height() * 2 );
    #endif

	return result;
}

/*
 * https://www.qtcentre.org/threads/53960-All-QStyleOptionButton-in-listview-sunken-when-clicking-one-of-them
 * https://stackoverflow.com/questions/26052818/get-qstandarditem-for-the-custom-context-menu-of-a-qtreeview
 */
bool SideBarListViewDelegate::editorEvent(
	QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
	Q_UNUSED( model );
	Q_UNUSED( option );

	switch ( event->type() ) {
		case QEvent::MouseButtonPress:
			{
				//qDebug() << "MouseButtonPress: " << _currRow;
				_currRow	= index.row();
				QMouseEvent* me = static_cast<QMouseEvent *>(event);
				//int currRow	= _currRow;
				_currRow	= index.row();
				if ( me->button() == Qt::RightButton )
				{
					_event	= 1;
				}
			}
			break;
		case QEvent::MouseButtonRelease:
			//qDebug() << "MouseButtonRelease: " << _currRow;
			_event	= 0;
			//_currRow	= index.row();
			emit buttonClicked( index );
			break;

		default:
			//qDebug() << "Default Event: " << _currRow;
			_currRow	= -1;
			break;
	}

	return true;
}
