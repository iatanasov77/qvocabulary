#ifndef WIDGET_ARCHIVEGROUPS_H
#define WIDGET_ARCHIVEGROUPS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "ui_VocabularyGroupsWidget.h"

#include "Entity/ArchiveGroup.h"

namespace Ui {
	class VocabularyGroupsWidget;
}

class ArchiveGroupsWidget : public QWidget
{
    Q_OBJECT

	public:
		ArchiveGroupsWidget( QWidget *parent = 0 );
		~ArchiveGroupsWidget();

		int currentGroup();
		int groupRow( int groupId );
		void setCurrentGroup( int groupId );

	public slots:
		void setCurrentGroup( const QModelIndex &index );
		void scrollTo( int groupId );

	signals:
			void currentGroupChanged( const QModelIndex &index );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initModel();

		Ui::VocabularyGroupsWidget* ui;
		qx::QxModel<ArchiveGroup>* pModel;
};

#endif // WIDGET_ARCHIVEGROUPS_H
