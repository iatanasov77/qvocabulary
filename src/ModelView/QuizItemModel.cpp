#include "ModelView/QuizItemModel.h"

#include <QObject>
#include <QModelIndex>

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

