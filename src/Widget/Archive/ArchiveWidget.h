#ifndef WIDGET_ARCHIVE_WORDS_H
#define WIDGET_ARCHIVE_WORDS_H

#include <QWidget>
#include <QListView>
#include <QTableView>
#include <QSplitter>
#include <QModelIndex>
#include <QMap>
#include <QVariant>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "ui_VocabularyWidget.h"

#include "Entity/ArchiveWord.h"
#include "Widget/Archive/ArchiveGroupsWidget.h"
#include "Widget/Archive/ArchiveWordsWidget.h"

namespace Ui {
	class VocabularyWidget;
}

class ArchiveWidget : public QWidget
{
    Q_OBJECT

	public:
		ArchiveWidget( QWidget *parent = 0 );
		~ArchiveWidget();

		void setCurrentGroupName( QString groupName );
		void refreshWidgets();
		void setCurrentGroup( int groupId );
		void showWord( int wordId, int groupId );

	public slots:
		void loadGroup( int groupId = 0 );
		void loadGroup( const QModelIndex &index );
		void updateSpeaker();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		void init();
		void initModels();

		Ui::VocabularyWidget *ui;
		QSplitter* horizSplitter;
		ArchiveGroupsWidget* wdgGroups;
		ArchiveWordsWidget* wdgWords;

		int currentGroup;
};

#endif // WIDGET_ARCHIVE_WORDS_H
