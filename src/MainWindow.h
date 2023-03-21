#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QSqlTableModel>

#include "Widget/Quiz/QuizListWindow.h"
#include "Widget/Quiz/QuizWindow.h"
#include "Widget/Help/HelpWindow.h"
#include "Widget/Archive/ArchiveWidget.h"
#include "Widget/Archive/ArchiveCompareWidget.h"
#include "Widget/Vocabulary/VocabularyWidget.h"
#include "Widget/Settings/SettingsWindow.h"
#include "Dialog/NewDatabaseDialog.h"
#include "Dialog/NewVocabularyGroupDialog.h"
#include "Dialog/RenameVocabularyGroupDialog.h"

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
		void setCurrentGroup( int groupId );
		void initWidgets();
		void initVocabularyWidget();
		void initArchiveWidget();
		//friend void NewVocabularyGroupDialog::reloadWidgets();
		//friend void RenameVocabularyGroupDialog::reloadWidgets();

		void showVocabularyWord( int wordId, int groupId );
		void showArchiveWord( int wordId, int groupId );
		void openQuiz( int quizId, QString quizTitle );

		NewVocabularyGroupDialog* dialogNewVocabularyGroup();

		void startQuiz( int groupId = 0 );

	protected:
		// this event is called, when a new translator is loaded or the system language is changed
		void changeEvent(QEvent*);
		virtual void resizeEvent( QResizeEvent* event );

	private slots:
		void on_actionNew_DB_triggered();
		void on_actionOpen_DB_triggered();
		void openRecentDatabase();
		void clearRecentDatabases();
		void on_actionSave_As_triggered();
		void on_actionPreferences_triggered();

		void on_actionHelp_triggered();
		void on_actionAbout_triggered();

		void on_actionImportMicrosoftVocabulary_triggered();
		void on_actionExportMicrosoftVocabulary_triggered();
		void on_actionImportVankoSoftQVocabulary_triggered();
		void on_actionImport_from_Android_MyDictionary_xls_triggered();

		void on_actionSimpleExam_triggered();
		void on_actionCompletedExams_triggered();

		void on_actionShow_Vocabulary_triggered();
		void on_actionShow_Archive_triggered();
		void on_actionAdd_to_Archive_triggered();
		void on_actionImport_Archive_triggered();
		void on_actionCompair_Vocabulary_Archive_triggered();
		void on_actionExec_SQL_Query_triggered();
		void on_actionTest_Widgets_triggered();

		void loadLanguage();

	public slots:
		void on_actionInsertGroup_triggered();
		void on_actionInsertWord_triggered();

	private:
		// Methods
		void initDebug();
		void stylingMenu();
		void initIcons();
		void initDatabase();
		void initMenuLanguages();
		void clearVocabularyWidget();
		void clearArchiveWidget();

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
		SettingsWindow* wdgSettings;
		QuizListWindow* wdgQuizList;
		QuizWindow* wdgQuiz;
		ArchiveWidget* wdgArchive;
		ArchiveCompareWidget* wdgCompair;
		//QSqlTableModel* modelCompair;

		QAction* actClearRecentDatabases;
		QAction* actClearRecentDatabasesWithoutCurrent;
		QAction* separatorAct;
		enum { MaxRecentDatabases = 5 };
		QString curDatabase;
		QAction* recentDatabaseActs[MaxRecentDatabases];

//		QDockWidget* helpWindow;
//		QDockWidget* quizWindow;
};

#endif // MAINWINDOW_H
