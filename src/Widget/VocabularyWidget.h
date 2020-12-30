#ifndef WIDGET_VOCABULARY_H
#define WIDGET_VOCABULARY_H

#include <QWidget>
#include <QListView>
#include <QTableView>
#include <QSplitter>
#include <QModelIndex>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/Vocabulary.h"

#include "Widget/VocabularyGroupsWidget.h"
#include "Widget/VocabularyWordsWidget.h"

namespace Ui {
	class VocabularyWidget;
}

class VocabularyWidget : public QWidget
{
    Q_OBJECT

	public:
		explicit VocabularyWidget( QWidget *parent = 0 );
		~VocabularyWidget();

		void initModels();
		void insertWord();
		void loadGroup( int groupId );
		void deleteGroup( int groupId );
		void setCurrentGroupName( QString groupName );
		void refreshWidgets();

	public slots:
		void loadGroup( const QModelIndex &index );

	private:
		void init();

		Ui::VocabularyWidget *ui;
		VocabularyGroupsWidget* wdgGroups;
		VocabularyWordsWidget* wdgWords;

		int currentGroup;
};

#endif // WIDGET_VOCABULARY_H
