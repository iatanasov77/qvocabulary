#include "Helper.h"

#include <QList>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

QMap<int, QString> Helper::getAllGroups()
{
	qx::QxModel<VocabularyGroup>* model	= new qx::QxModel<VocabularyGroup>();
	model->qxFetchAll();

	QMap<int, QString> groups;
	QModelIndex parent = QModelIndex();

	/*
	 * Tutorial: https://stackoverflow.com/questions/33124903/how-to-iterate-through-a-qstandarditemmodel-completely
	 */
	for( int r = 0; r < model->rowCount( parent ); ++r ) {

		QModelIndex index = model->index( r, 0, parent );
		int id 			= model->data( model->index( r, 0, parent ) ).toInt();
		QString name	= model->data( model->index( r, 1, parent ) ).toString();
		groups[id]	= name;

		if( model->hasChildren( index ) ) {
			//forEach( pModel, index );
		}
	}

	return groups;
}

void Helper::delay( int secs )
{
    QTime dieTime= QTime::currentTime().addSecs( secs );
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
}
