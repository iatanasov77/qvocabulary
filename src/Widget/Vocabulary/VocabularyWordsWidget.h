#ifndef WIDGET_VOCABULARYWORDS_H
#define WIDGET_VOCABULARYWORDS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>
#include <QTextToSpeech>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"
#include "Application/VsSpeaker.h"

namespace Ui {
	class VocabularyWordsWidget;
}

class VocabularyWordsWidget : public QWidget
{
    Q_OBJECT

	public:
		explicit VocabularyWordsWidget( QWidget *parent = 0 );
		~VocabularyWordsWidget();

		void setViewHeader( VocabularyMetaInfoPtr metaInfo );
		void insertWord();
		void loadGroup( int groupId );
		int deleteGroup( int groupId );
		void refreshView( QModelIndex topLeft, QModelIndex bottomRight );

	public slots:
		void onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
		void displayContextMenu( QPoint pos );
		void moveToGroup();
		void deleteWord();
		void search();
		void showTranscriptions( int state );
		void sayWord( const QModelIndex &index );

		void modelRowsInserted( const QModelIndex & parent, int start, int end );
		void updateSpeaker();
		void updateView();

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		QStringList viewHeaders( VocabularyMetaInfoPtr metaInfo );
		void initModel();
		void initContextMenu();
		void initTextToSpeech();
		void adjustRowSelection();
		void displaySearchResults( qx::QxModel<Vocabulary> *searchModel );

		int currentGroup;
		Ui::VocabularyWordsWidget *ui;
		qx::QxModel<Vocabulary> *pModel;
		QList<int> hideColumns;
		VsSpeaker* speeker;
};

#endif // WIDGET_VOCABULARYWORDS_H
