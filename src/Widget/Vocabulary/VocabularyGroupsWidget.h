#ifndef WIDGET_VOCABULARYGROUPS_H
#define WIDGET_VOCABULARYGROUPS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"
#include "Model/VocabularyGroupsModel.h"

namespace Ui {
	class VocabularyGroupsWidget;
}

class VocabularyGroupsWidget : public QWidget
{
    Q_OBJECT

	public:
		VocabularyGroupsWidget( QWidget *parent = 0 );
		~VocabularyGroupsWidget();

		void refreshView( QModelIndex topLeft, QModelIndex bottomRight );
		void refreshView();
		int currentGroup();
		int groupRow( int groupId );
		void setCurrentGroup( int groupId );
		int setCurrentGroup();

	public slots:
		void setCurrentGroup( const QModelIndex &index );
		void scrollTo( int groupId );
		void displayContextMenu( QPoint pos );
		void createGroup();
		void renameGroup();
		void deleteGroup();

		void modelRowsInserted( const QModelIndex & parent, int start, int end );

	signals:
		void currentGroupChanged( const QModelIndex &index );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initModel();
		void initContextMenu();

		Ui::VocabularyGroupsWidget* ui;

		//qx::QxModel<VocabularyGroup> *pModel;
		VocabularyGroupsModel *pModel;
};

#endif // WIDGET_VOCABULARYGROUPS_H
