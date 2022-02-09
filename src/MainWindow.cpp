#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QStringList>
#include <QSqlQueryModel>

// From: https://github.com/robertknight/qt-mustache
#include "../lib/QtMustache/mustache.h"

#include "Application/VsApplication.h"
#include "Application/VsSettings.h"
#include "Application/VsDatabase.h"
#include "Application/Import/MicrosoftVocabulary.h"
#include "Widget/Help/HelpWindow.h"
#include "Widget/Quiz/QuizListWindow.h"
#include "Widget/Quiz/QuizWindow.h"
#include "Widget/Vocabulary/VocabularyWidget.h"
#include "Widget/Settings/SettingsWindow.h"
#include "Dialog/InitDatabaseDialog.h"
#include "Dialog/NewDatabaseDialog.h"
#include "Dialog/NewVocabularyGroupDialog.h"
#include "Dialog/AddToArchiveDialog.h"
#include "Dialog/ImportArchiveDialog.h"
#include "Dialog/QVocabularyImportDialog.h"
#include "Dialog/MyDictionaryImportDialog.h"
#include "Dialog/ExecSqlQueryDialog.h"
#include "Dialog/TestWidgetsDialog.h"

MainWindow::MainWindow( QWidget *parent ) :
	QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    // The Trick ;)
    wdgVocabulary	= 0;
    wdgArchive		= 0;

    // Load Current Language
    QString currentLanguage = VsSettings::instance()->value( "language", "General" ).toString();
    if ( ! currentLanguage.isEmpty() ) {
    	VsApplication::instance()->loadLanguage( currentLanguage );
    }

    stylingMenu();
    initIcons();
    createReccentDatabaseActions();
    updateRecentDatabaseActions();
    initDatabase();
    initMenuLanguages();

    // Set MainWindow Size Previous Set In Settings
    VsSettings* settings	= VsSettings::instance();
    int mwWidth			= settings->value( "mainWindowWidth", "MainWindow" ).toInt();
    int mwHeight		= settings->value( "mainWindowHeight", "MainWindow" ).toInt();
    if ( mwWidth && mwHeight ) {
    	resize( mwWidth, mwHeight );
    }

    initDebug();

    connect( ui->actionAboutQt, SIGNAL( triggered() ), qApp, SLOT( aboutQt() ) );

    //connect( exitAction, &QAction::triggered, qApp, &QApplication::closeAllWindows, Qt::QueuedConnection );
	connect( ui->actionExit, SIGNAL( triggered() ), this, SLOT( close() ) );
	connect( ui->actionExit, SIGNAL( triggered() ), qApp, SLOT( quit() ) );
}

MainWindow::~MainWindow()
{
	delete wdgVocabulary;
	delete wdgArchive;
    delete ui;
}

void MainWindow::initDebug()
{
	#ifndef QT_DEBUG
		ui->actionExec_SQL_Query->setVisible( false );
		ui->actionTest_Widgets->setVisible( false );
	#endif
}

void MainWindow::initWidgets()
{
	initVocabularyWidget();
	initArchiveWidget();

	ui->stackedWidget->setCurrentWidget( ui->pageVocabulary );
}

void MainWindow::stylingMenu()
{
	// https://doc.qt.io/qt-5/stylesheet-examples.html
	ui->toolBar->setStyleSheet( "QToolBar::separator { width: 1px; background: black; margin: 3px 7px 3px 7px; }" );
	ui->menuBar->setStyleSheet( "QMenu::separator { height: 1px; background: black; margin: 7px 3px 7px 3px; }" );
}

void MainWindow::initIcons()
{
	setWindowIcon( QIcon( ":/Resources/icons/dictionary.svg" ) );

	//QIcon::setThemeName( "breeze" );
	ui->actionNew_DB->setIcon( QIcon( ":/Resources/icons/document-new.svg" ) );
	ui->actionOpen_DB->setIcon( QIcon( ":/Resources/icons/document-open.svg" ) );
	ui->menu_Open_Database_Recent->setIcon( QIcon( ":/Resources/icons/document-open-recent.svg" ) );
	ui->actionSave_As->setIcon( QIcon( ":/Resources/icons/computer.svg" ) );

	ui->menuImport->setIcon( QIcon( ":/Resources/icons/document-import.svg" ) );

	ui->actionSimpleExam->setIcon( QIcon( ":/Resources/icons/quiz.svg" ) );
	ui->actionCompletedExams->setIcon( QIcon( ":/Resources/icons/quiz-list.svg" ) );

	ui->actionInsertGroup->setIcon( QIcon( ":/Resources/icons/folder-new.svg" ) );
	ui->actionInsertWord->setIcon( QIcon( ":/Resources/icons/mail-message-new.svg" ) );

	ui->actionHelp->setIcon( QIcon( ":/Resources/icons/system-help.svg" ) );
	ui->actionAbout->setIcon( QIcon( ":/Resources/icons/dictionary.svg" ) );

	ui->actionExit->setIcon( QIcon( ":/Resources/icons/close.svg" ) );
	ui->actionAboutQt->setIcon( QIcon( ":/Resources/icons/QtProject-designer.svg" ) );

	ui->actionPreferences->setIcon( QIcon( ":/Resources/icons/settings.svg" ) );

	ui->actionShow_Vocabulary->setIcon( QIcon( ":/Resources/icons/dictionary.svg" ) );
	ui->actionShow_Archive->setIcon( QIcon( ":/Resources/icons/archive.svg" ) );
	ui->actionAdd_to_Archive->setIcon( QIcon( ":/Resources/icons/add_archive.svg" ) );
	ui->actionImport_Archive->setIcon( QIcon( ":/Resources/icons/document-import.svg" ) );
	ui->actionCompair_Vocabulary_Archive->setIcon( QIcon( ":/Resources/icons/compare.svg" ) );
}

void MainWindow::on_actionInsertGroup_triggered()
{
	NewVocabularyGroupDialog *dlg	= dialogNewVocabularyGroup();
	if( dlg->exec() == QDialog::Accepted )
	{
		wdgVocabulary->refreshGroups();
		//initVocabularyWidget();
		//setCurrentGroup( newGroupId );
	}
}

void MainWindow::on_actionInsertWord_triggered()
{
	wdgVocabulary->insertWord();
}

void MainWindow::on_actionNew_DB_triggered()
{
	QString dbPath	= createNewDatabase();
	loadDb( dbPath );
}

void MainWindow::on_actionOpen_DB_triggered()
{
	QString dbPath = openDatabase();

	if ( ! dbPath.isEmpty() ) {
		loadDb( dbPath );
	}
}

void MainWindow::on_actionAbout_triggered()
{
	QVariantHash aboutBody;
	aboutBody["version"]		= VsApplication::appVersion();
	aboutBody["build"]			= VsApplication::appBuildTime();
	//aboutBody["copySign"] 		= QString::fromUtf8( "\u00A9" );
	aboutBody["currentYear"]	= QDate::currentDate().year();

	Mustache::Renderer renderer;
	Mustache::QtVariantContext context( aboutBody );

	QString aboutText = renderer.render( VsApplication::instance()->appAboutBody(), &context );

	QMessageBox::about(
		this,
		QObject::tr( "About QVocabulary" ),
		aboutText	//QObject::tr( qPrintable( aboutText ) )
	);
}

void MainWindow::on_actionHelp_triggered()
{
	wdgHelp = new HelpWindow( this );
	wdgHelp->setWindowFlags( Qt::Window );
	//wdgHelp->setModal( true );
	wdgHelp->show();
}

void MainWindow::on_actionPreferences_triggered()
{
	wdgSettings = new SettingsWindow( this );
	wdgSettings->setWindowFlags( Qt::Window );
	//wdgHelp->setModal( true );

	connect( wdgSettings, SIGNAL( speakerSettingsUpdated() ), wdgVocabulary, SLOT( updateSpeaker() ) );

	wdgSettings->show();
}

void MainWindow::openRecentDatabase()
{
    QAction *action = qobject_cast<QAction *>( sender() );
    if ( action ) {
        loadDb( action->data().toString() );
    }
}

void MainWindow::clearRecentDatabases()
{
	QStringList emptyList;
	QAction *action		= qobject_cast<QAction *>( sender() );
	bool withCurrent	= action->data().toBool();

	if ( withCurrent ) {
		emptyList	= QStringList();
	} else {
		QStringList databases	= VsSettings::instance()->value( "recentDatabaseList", "MainWindow" ).toStringList();
		emptyList				= QStringList( QStringList() << databases.at( 0 ) );
	}
	VsSettings::instance()->setValue( "recentDatabaseList", emptyList, "MainWindow" );

    updateRecentDatabaseActions();
}

void MainWindow::createReccentDatabaseActions()
{
	for ( int i = 0; i < MaxRecentDatabases; ++i ) {
		recentDatabaseActs[i] = new QAction( this );
		recentDatabaseActs[i]->setVisible( false );
		connect( recentDatabaseActs[i], SIGNAL( triggered() ),this, SLOT( openRecentDatabase() ) );
	}

	for ( int i = 0; i < MaxRecentDatabases; ++i )
		ui->menu_Open_Database_Recent->addAction( recentDatabaseActs[i] );

	actClearRecentDatabases	= new QAction( this );
	actClearRecentDatabases->setText( tr( "Clear Recent" ) );
	actClearRecentDatabases->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	actClearRecentDatabases->setVisible( false );
	actClearRecentDatabases->setData( QVariant( true ) );	// With Current Database Also
	connect( actClearRecentDatabases, SIGNAL( triggered() ),this, SLOT( clearRecentDatabases() ) );

	actClearRecentDatabasesWithoutCurrent	= new QAction( this );
	actClearRecentDatabasesWithoutCurrent->setText( tr( "Clear Recent Without Current" ) );
	actClearRecentDatabasesWithoutCurrent->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	actClearRecentDatabasesWithoutCurrent->setVisible( false );
	actClearRecentDatabasesWithoutCurrent->setData( QVariant( false ) );	// Without Current Database
	connect( actClearRecentDatabasesWithoutCurrent, SIGNAL( triggered() ),this, SLOT( clearRecentDatabases() ) );

	separatorAct = ui->menu_Open_Database_Recent->addSeparator();
	ui->menu_Open_Database_Recent->addAction( actClearRecentDatabases );
	ui->menu_Open_Database_Recent->addAction( actClearRecentDatabasesWithoutCurrent );
}

void MainWindow::updateRecentDatabaseActions()
{
    QStringList databases	= VsSettings::instance()->value( "recentDatabaseList", "MainWindow" ).toStringList();

    int numRecentDatabases	= qMin( databases.size(), (int)MaxRecentDatabases );

    for ( int i = 0; i < numRecentDatabases; ++i ) {
        QString text = tr( "&%1 %2").arg( i + 1 ).arg( strippedName( databases[i] ) );
        recentDatabaseActs[i]->setText( text );
        recentDatabaseActs[i]->setData( databases[i] );
        recentDatabaseActs[i]->setVisible( true );
    }

    for ( int j = numRecentDatabases; j < MaxRecentDatabases; ++j )
        recentDatabaseActs[j]->setVisible( false );

    ui->actionEmpty->setVisible( numRecentDatabases == 0 );
    separatorAct->setVisible( numRecentDatabases > 0 );
    actClearRecentDatabases->setVisible( numRecentDatabases > 0 );
    actClearRecentDatabasesWithoutCurrent->setVisible( numRecentDatabases > 1 );
}

QString MainWindow::strippedName( const QString &fullDbPath )
{
	QFileInfo fi( fullDbPath );

    return "..." + fi.absolutePath().right( 20 ) + "/" + fi.fileName();
}

void MainWindow::on_actionSave_As_triggered()
{
	QString dbPath	= QFileDialog::getSaveFileName(
							this,
							tr( "Create Database" ),
							curDatabase,
							tr( "*.db" )
						);

	// An easy and safe way to do it, use the STD libraries, std::ifstream is a good option:
	std::ifstream  src( curDatabase.toStdString().c_str(), std::ios::binary );
	std::ofstream  dst( dbPath.toStdString().c_str(),   std::ios::binary );
	dst << src.rdbuf();

	loadDb( dbPath );
}

void MainWindow::loadDb( const QString &dbPath )
{
	if ( curDatabase == dbPath ) return;

	if ( ! QFile::exists( dbPath ) ) {
		QMessageBox::warning( this, tr( "Recent Databases" ),
			tr( "Cannot read database: %1." )
			.arg( dbPath )
		);

		return;
	}

	VsDatabase::instance()->connect( dbPath );
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();

	// Check If can Open DB
	if ( VsApplication::canOpenDb( metaInfo->dbVersion ) == false ) {
		// Clear Recent Databases List and call initDatabase() again
		VsSettings::instance()->setValue( "recentDatabaseList", QStringList(), "MainWindow" );
		initDatabase();
	}

	// Check for DB Version
	if ( metaInfo->dbVersion != VsApplication::DB_VERSION ) {
		QMessageBox::warning(
			this,
			tr( "Warning" ),
			tr( "The Current Database Version is Different from The Database Version used for the creation of this Database !" )
		);
	}

	QApplication::setOverrideCursor( Qt::WaitCursor );

	initWidgets();
	wdgVocabulary->initModels();
	setCurrentDb( dbPath );

	QApplication::restoreOverrideCursor();
	statusBar()->showMessage( tr( "Database loaded" ), 2000 );
}

void MainWindow::setCurrentDb( const QString &dbPath )
{
    curDatabase = dbPath;
    setWindowFilePath( curDatabase );

    QStringList databases	= VsSettings::instance()->value( "recentDatabaseList", "MainWindow" ).toStringList();
    databases.removeAll( dbPath );
    databases.prepend( dbPath );
    while ( databases.size() > MaxRecentDatabases )
        databases.removeLast();

    VsSettings::instance()->setValue( "recentDatabaseList", QVariant( databases ), "MainWindow" );

    updateRecentDatabaseActions();
}

void MainWindow::initDatabase()
{
	QString dbPath;

	QStringList files	= VsSettings::instance()->value( "recentDatabaseList", "MainWindow" ).toStringList();
	if ( files.size() && QFile::exists( files.at( 0 ) ) ) {
		dbPath	= files.at( 0 );
	} else {
		InitDatabaseDialog *dlg	= new InitDatabaseDialog( this );

		dlg->setModal( true );
		if ( dlg->exec() == QDialog::Accepted )
		{
			if ( dlg->initDatabase() == InitDatabaseDialog::NewDatabase ) {
				dbPath	= createNewDatabase();
			}

			if ( dlg->initDatabase() == InitDatabaseDialog::OpenDatabase ) {
				dbPath	= openDatabase();
				//qDebug() << "Open DB: " << dbPath;
			}
		}
	}

	//qDebug() << "Load DB: " << dbPath;
	loadDb( dbPath );
}

QString MainWindow::openDatabase()
{
	QString dbPath;

	QStringList files	= VsSettings::instance()->value( "recentDatabaseList", "MainWindow" ).toStringList();
	if ( files.size() && QFile::exists( files.at( 0 ) ) ) {
		dbPath	= QFileInfo( files.at( 0 ) ).absolutePath();
	} else {
		dbPath	= QDir::homePath();
	}

	return QFileDialog::getOpenFileName( this, tr( "Open Database" ), dbPath, tr( "Db Files (*.db)" ) );
}

QString MainWindow::createNewDatabase()
{
	dlgNewDatabase	= new NewDatabaseDialog( this );
	dlgNewDatabase->setModal( true );
	if ( dlgNewDatabase->exec() == QDialog::Accepted )
		return dlgNewDatabase->database();

	return "UNSELECTED DATABASE";
}

void MainWindow::on_actionImportMicrosoftVocabulary_triggered()
{
	QString xmlFile = QFileDialog::getOpenFileName(
		this,
		tr( "Open Microsoft Vocabulary++ Dictionary" ),
		QDir::homePath(),
		tr( "Microsoft Vocabulary Files (*.vocab)" )
	);

	if ( ! xmlFile.isEmpty() ) {
		MicrosoftVocabulary::importFromFile( xmlFile );

		initWidgets();
		wdgVocabulary->initModels();
		statusBar()->showMessage( tr( "Database imported" ), 2000 );
	}
}

void MainWindow::on_actionExportMicrosoftVocabulary_triggered()
{
	QString xmlFile = QFileDialog::getSaveFileName(
		this,
		tr( "Export to Microsoft Vocabulary++ Dictionary" ),
		QDir::homePath() + "/untitled.vocab",
		tr( "Microsoft Vocabulary Files (*.vocab)" )
	);

	if ( ! xmlFile.isEmpty() ) {
		MicrosoftVocabulary::exportToFile( xmlFile );

		statusBar()->showMessage( tr( "Database exported" ), 2000 );
	}
}

void MainWindow::on_actionImportVankoSoftQVocabulary_triggered()
{
	QVocabularyImportDialog* dlgImport	= new QVocabularyImportDialog( this );
	dlgImport->setModal( true );
	if ( dlgImport->exec() == QDialog::Accepted ) {
		initWidgets();
		wdgVocabulary->initModels();
		statusBar()->showMessage( tr( "Database imported" ), 2000 );
	}
}

void MainWindow::on_actionSimpleExam_triggered()
{
	wdgQuiz = new QuizWindow( this );
	wdgQuiz->setWindowFlags( Qt::Window );
	//wdgQuiz->setModal( true );
	wdgQuiz->show();
}

void MainWindow::on_actionCompletedExams_triggered()
{
	wdgQuizList = new QuizListWindow( this );
	wdgQuizList->setWindowFlags( Qt::Window );
	//wdgQuiz->setModal( true );
	wdgQuizList->show();
}

void MainWindow::openQuiz( int quizId, QString quizTitle )
{
	// Be Sure That wdgQuiz is Opened
	wdgQuiz->hide();
	delete wdgQuiz;

	on_actionCompletedExams_triggered();
	wdgQuizList->displayItems( quizId, quizTitle );
}

void MainWindow::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        // retranslate designer form (single inheritance approach)
        ui->retranslateUi( this );
        initMenuLanguages();

        //QString languageName	= QLocale::languageToString( QLocale().language() );
        statusBar()->showMessage( tr( "Current Language changed to %1" ).arg( QLocale().nativeLanguageName() ), 2000 );
    }

    // remember to call base class implementation
    QMainWindow::changeEvent( event );
}

void MainWindow::initMenuLanguages()
{
	QMap<QString, QString> languages	= VsApplication::instance()->languages();

	ui->menuLanguages->clear();
	foreach ( QString key, languages.keys() ) {
		QAction* act	= new QAction( this );
		act->setText( qApp->translate( "Vocabulary", languages[key].toStdString().c_str() ) );
		act->setData( QVariant( key ) );

		connect( act, SIGNAL( triggered() ), this, SLOT( loadLanguage() ) );
		ui->menuLanguages->addAction( act );
	}
}

void MainWindow::loadLanguage()
{
	QAction *action = qobject_cast<QAction *>( sender() );

	if ( action ) {
		VsApplication::instance()->loadLanguage( action->data().toString() );
	}
}

/**
 * Called From NewVocabularyGroupDialog
 */
void MainWindow::setCurrentGroup( int groupId )
{
	wdgVocabulary->setCurrentGroup( groupId );
}

void MainWindow::resizeEvent( QResizeEvent* event )
{
	QMainWindow::resizeEvent( event );

	VsSettings* settings	= VsSettings::instance();
	QSize mwSize		= event->size(); // Returns the new size of the widget. This is the same as QWidget::size().

	settings->setValue( "mainWindowWidth", QVariant( mwSize.width() ), "MainWindow" );
	settings->setValue( "mainWindowHeight", QVariant( mwSize.height() ), "MainWindow" );
}

void MainWindow::on_actionShow_Vocabulary_triggered()
{
	ui->stackedWidget->setCurrentWidget( ui->pageVocabulary );
}

void MainWindow::on_actionShow_Archive_triggered()
{
	ui->stackedWidget->setCurrentWidget( ui->pageArchive );
}

void MainWindow::on_actionAdd_to_Archive_triggered()
{
	AddToArchiveDialog* dlgAddArchive	= new AddToArchiveDialog( this );
	dlgAddArchive->setModal( true );
	if ( dlgAddArchive->exec() == QDialog::Accepted ) {
		initArchiveWidget();
	}
}

void MainWindow::on_actionImport_Archive_triggered()
{
	ImportArchiveDialog* dlgImportArchive	= new ImportArchiveDialog( this );
	dlgImportArchive->setModal( true );
	if ( dlgImportArchive->exec() == QDialog::Accepted ) {
		initArchiveWidget();
	}
}

void MainWindow::on_actionExec_SQL_Query_triggered()
{
	ExecSqlQueryDialog* dlgSqlQuery	= new ExecSqlQueryDialog( this );
	dlgSqlQuery->setModal( true );
	dlgSqlQuery->exec();
}

void MainWindow::clearVocabularyWidget()
{
	while( ! ui->verticalLayout_5->isEmpty() ) {
		QWidget *w = ui->verticalLayout_5->takeAt( 0 )->widget();
		delete w;
	}
}

void MainWindow::clearArchiveWidget()
{
	while( ! ui->verticalLayout_4->isEmpty() ) {
		QWidget *w = ui->verticalLayout_4->takeAt( 0 )->widget();
		delete w;
	}
}

void MainWindow::initVocabularyWidget()
{
	QMap<QString, QVariant> widgetState;

	if ( wdgVocabulary ) {
		widgetState	= wdgVocabulary->getState();
	}

	clearVocabularyWidget();
	wdgVocabulary	= new VocabularyWidget( this );

	if ( ! widgetState.isEmpty() ) {
		wdgVocabulary->setState( widgetState );
	}
	wdgVocabulary->setAcceptDrops( true );

	ui->verticalLayout_5->addWidget( wdgVocabulary );
}

void MainWindow::initArchiveWidget()
{
	clearArchiveWidget();

	wdgArchive	= new ArchiveWidget( this );

	ui->verticalLayout_4->addWidget( wdgArchive );
}

void MainWindow::on_actionCompair_Vocabulary_Archive_triggered()
{
	while( ! ui->verticalLayout_7->isEmpty() ) {
		QWidget *w = ui->verticalLayout_7->takeAt( 0 )->widget();
		delete w;
	}

	wdgCompair	= new ArchiveCompareWidget( this );
	ui->verticalLayout_7->addWidget( wdgCompair );
	ui->stackedWidget->setCurrentWidget( ui->pageCompairArchive );
}

void MainWindow::on_actionTest_Widgets_triggered()
{
	TestWidgetsDialog* dlgTestWidgets	= new TestWidgetsDialog( this );
	dlgTestWidgets->setModal( true );
	dlgTestWidgets->exec();
}

void MainWindow::showVocabularyWord( int wordId, int groupId )
{
	on_actionShow_Vocabulary_triggered();
	wdgVocabulary->showWord( wordId, groupId );
}

void MainWindow::showArchiveWord( int wordId, int groupId )
{
	on_actionShow_Archive_triggered();
	wdgArchive->showWord( wordId, groupId  );
}

NewVocabularyGroupDialog* MainWindow::dialogNewVocabularyGroup()
{
	NewVocabularyGroupDialog *dlg	= new NewVocabularyGroupDialog( this );
	dlg->setModal( true );

	return dlg;
}

void MainWindow::on_actionImport_from_Android_MyDictionary_xls_triggered()
{
	MyDictionaryImportDialog* dlgImport	= new MyDictionaryImportDialog( this );
	dlgImport->setModal( true );
	if ( dlgImport->exec() == QDialog::Accepted ) {
		initWidgets();
		wdgVocabulary->initModels();
		statusBar()->showMessage( tr( "Database imported" ), 2000 );
	}
}
