#ifndef WIDGET_ARCHIVEWORDS_H
#define WIDGET_ARCHIVEWORDS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>
#include <QTextToSpeech>
#include <QTreeWidgetItem>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "ui_VocabularyWordsWidget.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/ArchiveWord.h"
#include "Application/VsSpeaker.h"

namespace Ui {
	class VocabularyWordsWidget;
}

class ArchiveWordsWidget : public QWidget
{
    Q_OBJECT

	public:
		ArchiveWordsWidget( QWidget *parent );
		~ArchiveWordsWidget();

		void setViewHeader( VocabularyMetaInfoPtr metaInfo );
		void loadGroup( int groupId );
		int deleteGroup( int groupId );
		void refreshView( QModelIndex topLeft, QModelIndex bottomRight );

	public slots:
		void search();
		void showWord( QTreeWidgetItem* item, int column );
		void sayWord( const QModelIndex &index );

		void updateSpeaker();
		void updateView();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		QStringList viewHeaders( VocabularyMetaInfoPtr metaInfo );
		void initView();
		void initModel();
		void initTextToSpeech();
		void displaySearchResults( qx::QxModel<ArchiveWord> *searchModel );

		int currentGroup;
		Ui::VocabularyWordsWidget *ui;
		qx::QxModel<ArchiveWord> *pModel;
		QList<int> hideColumns;
		VsSpeaker* speeker;

		qx::QxModel<ArchiveWord>* searchModel;
		QWidget* wdgVocabulary;
};

#endif // WIDGET_ARCHIVEWORDS_H
