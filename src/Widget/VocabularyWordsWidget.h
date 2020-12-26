#ifndef WIDGET_VOCABULARYWORDS_H
#define WIDGET_VOCABULARYWORDS_H

#include <QWidget>
#include <QListView>
#include <QModelIndex>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyMetaInfo.h"
#include "Entity/Vocabulary.h"

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

	private:
		void initModel();
		void initContextMenu();
		void displaySearchResults( qx::QxModel<Vocabulary> *searchModel );

		int currentGroup;
		Ui::VocabularyWordsWidget *ui;
		qx::QxModel<Vocabulary> *pModel;
		QList<int> hideColumns;
};

#endif // WIDGET_VOCABULARYWORDS_H
