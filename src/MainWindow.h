#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "Widget/QuizWindow.h"
#include "Widget/HelpWindow.h"
#include "Widget/VocabularyWidget.h"
#include "Dialog/NewDatabaseDialog.h"
//#include "Dialog/NewVocabularyGroupDialog.h"
//#include "Dialog/RenameVocabularyGroupDialog.h"

namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow( QWidget *parent = 0 );
		~MainWindow();

		void loadDb( const QString &dbPath );
		void initWidgets();
		//friend void NewVocabularyGroupDialog::reloadWidgets();
		//friend void RenameVocabularyGroupDialog::reloadWidgets();

	private slots:
		void on_actionNew_DB_triggered();
		void on_actionOpen_DB_triggered();
		void on_actionInsertGroup_triggered();
		void on_actionInsertWord_triggered();
		void openRecentDatabase();
		void clearRecentDatabases();

		void on_actionHelp_triggered();
		void on_actionAbout_triggered();

		void on_actionImportMicrosoftVocabulary_triggered();
		void on_actionSimple_Exam_triggered();

	private:
		// Methods
		void initIcons();
		void initDatabase();

		void createReccentDatabaseActions();

		void setCurrentDb( const QString &dbPath );
		void updateRecentDatabaseActions();
		QString strippedName( const QString &fullDbPath );
		QString openDatabase();
		QString createNewDatabase();

		// Properties
		Ui::MainWindow* ui;
		VocabularyWidget* wdgVocabulary;
		NewDatabaseDialog* dlgNewDatabase;
		HelpWindow* wdgHelp;
		QuizWindow* wdgQuiz;

		QAction* actClearRecentDatabases;
		QAction* separatorAct;
		enum { MaxRecentDatabases = 5 };
		QString curDatabase;
		QAction* recentDatabaseActs[MaxRecentDatabases];

//		QDockWidget* helpWindow;
//		QDockWidget* quizWindow;
};

#endif // MAINWINDOW_H
