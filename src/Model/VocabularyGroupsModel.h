#ifndef MODEL_VOCABULARYGROUPS_H
#define MODEL_VOCABULARYGROUPS_H

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

class VocabularyGroupsModel : public qx::QxModel<VocabularyGroup>
{
	Q_OBJECT

	public:
		VocabularyGroupsModel( bool showAllButton = false, QObject* parent = nullptr );
	    virtual ~VocabularyGroupsModel();

		QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const;
		virtual QSqlError qxFetchAll (
			const QStringList &relation = QStringList(),
			QSqlDatabase * 	pDatabase = NULL
		);

		bool showAllButton();

	private:
		bool _showAllButton;

		void createShowAllButton();

};

#endif	// MODEL_VOCABULARYGROUPS_H
