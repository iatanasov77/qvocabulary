#ifndef WIDGET_VOCABULARYGROUPS_H
#define WIDGET_VOCABULARYGROUPS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

namespace Ui {
	class VocabularyGroupsWidget;
}

class VocabularyGroupsWidget : public QWidget
{
    Q_OBJECT

	public:
		explicit VocabularyGroupsWidget( QWidget *parent = 0 );
		~VocabularyGroupsWidget();

		void refreshView( QModelIndex topLeft, QModelIndex bottomRight );
		void refreshView();
		int currentGroup();

	public slots:
		void setCurrentGroup( const QModelIndex &index );
		void displayContextMenu( QPoint pos );
		void renameGroup();
		void deleteGroup();

		void modelRowsInserted( const QModelIndex & parent, int start, int end );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void initModel();
		void initContextMenu();

		Ui::VocabularyGroupsWidget* ui;
		qx::QxModel<VocabularyGroup>* pModel;
};

#endif // WIDGET_VOCABULARYGROUPS_H
