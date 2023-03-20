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
		buttonText 		= tr( "Show All Words" );
	} else {
		long wordsCount	= groupWordsCount( index.siblingAtColumn( 0 ).data().toInt() );
		buttonText 		= QString( "%1 (%2)" ).arg( index.siblingAtColumn( 1 ).data().toString() ).arg( wordsCount );
	}

	QStyleOptionButton button	= createButton( index.row(), option.rect, buttonText );
	//qDebug() << "Draw Button at row " << index.row() << " with state " << QString::number( button.state );

	QStyleOptionButton quizButton	= createQuizButton( index.row(), quizButtonRect( option.rect ) );

	painter->save();
	QApplication::style()->drawControl( QStyle::CE_PushButton, &button, painter );
	QApplication::style()->drawControl( QStyle::CE_PushButton, &quizButton, painter );
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

	QRect btnQuizRect;
	QRect btnGroupRect;
	QMouseEvent* e	= ( QMouseEvent* )event;
	int clickX		= e->x();
	int clickY		= e->y();

	//qDebug() << "Event Type 2: " << event->type();
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
				_event		= QEvent::MouseButtonRelease;

				btnQuizRect	= quizButtonRect( option.rect );
				if(
					( clickX > btnQuizRect.x() && clickX < btnQuizRect.x() + btnQuizRect.width() ) &&
					( clickY > btnQuizRect.y() && clickY < btnQuizRect.y() + btnQuizRect.height() )
				) {
					emit quizButtonClicked( index );
				}


				btnGroupRect	= groupButtonRect( option.rect );
				if(
					( clickX > btnGroupRect.x() && clickX < btnGroupRect.x() + btnGroupRect.width() ) &&
					( clickY > btnGroupRect.y() && clickY < btnGroupRect.y() + btnGroupRect.height() )
				) {
					if ( _currRow == index.row() ) {
						QModelIndex newIndex 	= model->index( index.row(), 1 );
						emit buttonClicked( index );
						emit model->dataChanged( newIndex, newIndex );
					} else {
						QModelIndex oldIndex 	= model->index( _currRow, 1 );
						_currRow	= index.row();
						emit buttonClicked( index );
						emit model->dataChanged( oldIndex, oldIndex );
					}
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

QStyleOptionButton SideBarListViewDelegate::createQuizButton( int indexRow, QRect rect ) const
{
	QStyleOptionButton button;

	button.rect		= rect;
	button.icon 	= QIcon( ":/Resources/icons/quiz.svg" );
	button.iconSize	= QSize( 16, 16 );
	button.state 	= QStyle::State_Enabled;
	//button.text		= "LKVBSFKDBLF";

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

QRect SideBarListViewDelegate::groupButtonRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x = cellRect.left();
	int y = cellRect.top();
	int w = cellRect.width() - btnSize;
	int h = cellRect.height();

	return QRect( x, y, w, h );
}

QRect SideBarListViewDelegate::quizButtonRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x = cellRect.left() + cellRect.width() - btnSize;
	int y = cellRect.top();
	int w = btnSize;
	int h = btnSize;

	return QRect( x, y, w, h );
}
