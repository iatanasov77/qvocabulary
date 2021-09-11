#include "VocabularyGroupsModel.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

VocabularyGroupsModel::VocabularyGroupsModel( bool showAllButton, QObject* parent ) : QxModel<VocabularyGroup>( parent )
{
	 _showAllButton	=  showAllButton;
}

VocabularyGroupsModel::~VocabularyGroupsModel()
{

}

QVariant VocabularyGroupsModel::data( const QModelIndex &index, int role ) const
{
	return QxModel<VocabularyGroup>::data( index, role );
}

QSqlError VocabularyGroupsModel::qxFetchAll ( const QStringList &relation, QSqlDatabase *pDatabase )
{
	QSqlError error	= qx::QxModel<VocabularyGroup>::qxFetchAll ( relation, pDatabase );
	if ( _showAllButton ) {
		createShowAllButton();
	}

	return error;
}

bool VocabularyGroupsModel::showAllButton()
{
	return _showAllButton;
}

void VocabularyGroupsModel::createShowAllButton()
{
	insertRow( 0, QModelIndex() );
}
