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

#include "Widget/Vocabulary/VocabularyGroupsWidget.h"
#include "Widget/Vocabulary/VocabularyWordsWidget.h"

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
		void setCurrentGroup( int groupId );

	public slots:
		void loadGroup( const QModelIndex &index );
		void updateSpeaker();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void init();

		Ui::VocabularyWidget *ui;
		VocabularyGroupsWidget* wdgGroups;
		VocabularyWordsWidget* wdgWords;

		int currentGroup;
};

#endif // WIDGET_VOCABULARY_H
