#include "QuizItemModel.h"

#include <QObject>
#include <QModelIndex>
#include <QPixmap>
#include <QIcon>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/QuizItem.h"

QuizItemModel::QuizItemModel( QObject* parent ) : QxModel<QuizItem>( parent )
{

}

QuizItemModel::~QuizItemModel()
{

}

Qt::ItemFlags QuizItemModel::flags( const QModelIndex & index ) const
{
	if ( index.column() == 4 )
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
	else
		return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

//void QuizItemModel::setAnswerIcon( const QModelIndex& index, bool right )
//{
//	//static QPixmap s_rightIcon( ":/Resources/icons/amarok_cart_remove.svg" );
//	//static QPixmap s_wrongIcon( ":/Resources/icons/amarok_cart_remove.svg" );
//	//QPixmap icon(s_invalidIcon);
//	//QPixmap icon;
//
//	static QIcon s_rightIcon( ":/Resources/icons/amarok_cart_remove.svg" );
//	static QIcon s_wrongIcon( ":/Resources/icons/amarok_cart_remove.svg" );
//	QIcon icon;
//
//	if( right ) {
//		icon = s_rightIcon;
//	} else {
//		icon = s_wrongIcon;
//	}
//
//	setData( index, icon, Qt::DecorationRole );
//}
