#include "SideBarListViewDelegate.h"

#include <QApplication>
#include <QtGui>
#include <QSize>
#include <QListView>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "Entity/Vocabulary.h"
#include "Entity/ArchiveWord.h"

#include "Model/VocabularyGroupsModel.h"

SideBarListViewDelegate::SideBarListViewDelegate(  QAbstractItemModel *model, int currRow, bool inArchive, QObject *parent ) :
    QItemDelegate( parent )
{
	_inArchive	= inArchive;
	_currRow	= currRow;
	_event		= QEvent::None;

	_showAllButton	= _inArchive ? false : qobject_cast<VocabularyGroupsModel*>( model )->showAllButton();
}

void SideBarListViewDelegate::setEvent( int event )	// Pass a valid QEvent::Type
{
	_event	= event;
}

void SideBarListViewDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const
{
	if ( ! index.isValid() )
		return;

	QString buttonText;
	if ( _showAllButton && index.row() == 0 ) {
		buttonText 		= "Show All Words";
	} else {
		long wordsCount	= groupWordsCount( index.siblingAtColumn( 0 ).data().toInt() );
		buttonText 		= QString( "%1 (%2)" ).arg( index.siblingAtColumn( 1 ).data().toString() ).arg( wordsCount );
	}

	QStyleOptionButton button	= createButton( index.row(), option.rect, buttonText );
	//qDebug() << "Draw Button at row " << index.row() << " with state " << QString::number( button.state );

	painter->save();
	QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter );
	painter->restore();
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

	qDebug() << "Event Type 2: " << event->type();
	switch ( event->type() ) {
		case QEvent::MouseButtonPress:
			{
				QMouseEvent* me = static_cast<QMouseEvent *>(event);
				if ( me->button() == Qt::RightButton )
				{
					_event		= -1;
				} else {
					_event		= QEvent::MouseButtonRelease;
					//_currRow	= index.row();
				}
			}
			break;
		case QEvent::None:
		case QEvent::MouseButtonRelease:
			{
				_event	= QEvent::MouseButtonRelease;

				if ( _currRow == index.row() ) {
					QModelIndex newIndex 	= model->index( index.row(), 1 );
					emit buttonClicked( index );
					emit model->dataChanged( newIndex, newIndex );
				} else {
					QModelIndex oldIndex 	= model->index( _currRow, 1 );
					_currRow	= index.row();
					emit model->dataChanged( oldIndex, oldIndex );
					emit buttonClicked( index );
				}
			}
			break;
		default:
			//_currRow	= -1;
			break;
	}

	return true;
}

QStyleOptionButton SideBarListViewDelegate::createButton( int indexRow, QRect rect, QString text ) const
{
	QStyleOptionButton button;
	button.rect 	= rect;
	//button.setCursor( QCursor( Qt::PointingHandCursor ) );
	button.text 	= text;

	// Set Button State
	//qDebug() << "Event Type 1: " << _event;
	if( _currRow == indexRow ) {
		button.state	= QStyle::State_Sunken | QStyle::State_Enabled;
	} else {
		button.state	= QStyle::State_Raised | QStyle::State_Enabled;
	}

	return button;
}

long SideBarListViewDelegate::groupWordsCount( int groupId ) const
{
	QString query	= QString( "WHERE group_id=%1" ).arg( groupId );

	if ( _inArchive ) {
		return qx::dao::count<ArchiveWord>( qx::QxSqlQuery( query ) );
	} else {
		return qx::dao::count<Vocabulary>( qx::QxSqlQuery( query ) );
	}
}