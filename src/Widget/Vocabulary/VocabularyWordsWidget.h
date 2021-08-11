#ifndef WIDGET_VOCABULARYWORDS_H
#define WIDGET_VOCABULARYWORDS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>
#include <QTextToSpeech>
#include <QTreeWidgetItem>

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
		VocabularyWordsWidget( QWidget *parent );
		~VocabularyWordsWidget();

		void setViewHeader( VocabularyMetaInfoPtr metaInfo );
		int insertWord();
		void loadGroup( int groupId );
		int deleteGroup( int groupId );
		void refreshView( QModelIndex topLeft, QModelIndex bottomRight );
		QMap<QString, QVariant> getState();
		void setState( QMap<QString, QVariant> state );
		void showWord( int wordId, int groupId );

	public slots:
		void onDataChanged( const QModelIndex& topLeft, const QModelIndex& bottomRight );
		void displayContextMenu( QPoint pos );
		void moveToGroup();
		void deleteWord();
		void search();
		void showWord( QTreeWidgetItem* item, int column );
		void showTranscriptions( int state );
		void sayWord( const QModelIndex &index );
		void addWordDescription();

		void modelRowsInserted( const QModelIndex & parent, int start, int end );
		void updateSpeaker();
		void updateView();
		void saveHeaderSizes( int logicalIndex, int oldSize, int newSize );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent( QEvent* );

	private:
		QStringList viewHeaders( VocabularyMetaInfoPtr metaInfo );
		void initView();
		void initModel();
		void initContextMenu();
		void initTextToSpeech();
		void adjustRowSelection();
		void displaySearchResults( qx::QxModel<Vocabulary> *searchModel );
		bool insertFromEmptyRow( QModelIndex index );
		void restoreHeaderSizes();

		int currentGroup;
		Ui::VocabularyWordsWidget *ui;
		qx::QxModel<Vocabulary> *pModel;
		QList<int> hideColumns;
		VsSpeaker* speeker;

		qx::QxModel<Vocabulary>* searchModel;
		QWidget* wdgVocabulary;
};

#endif // WIDGET_VOCABULARYWORDS_H
