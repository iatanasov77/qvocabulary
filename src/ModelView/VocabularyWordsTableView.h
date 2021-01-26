#ifndef VIEW_VOCABULARYWORDSTABLEVIEW_H
#define VIEW_VOCABULARYWORDSTABLEVIEW_H

#include <QTableView>
#include <QDragMoveEvent>
#include <QDropEvent>

class VocabularyWordsTableView : public QTableView
{
	Q_OBJECT

	public:
		VocabularyWordsTableView( QWidget* parent = nullptr );
		~VocabularyWordsTableView();

	protected:
		void dragEnterEvent(QDragEnterEvent *event) override;
		void dragMoveEvent(QDragMoveEvent *event) override;
		void dragLeaveEvent(QDragLeaveEvent *event) override;
		void dropEvent(QDropEvent *event) override;
};

#endif	// VIEW_VOCABULARYWORDSTABLEVIEW_H
