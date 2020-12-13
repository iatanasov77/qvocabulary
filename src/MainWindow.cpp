#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QIcon>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>
#include <QSettings>
#include <QStandardPaths>
#include <QStringList>

#include "Application/VsApplication.h"
#include "Application/VsSettings.h"
#include "Application/VsDatabase.h"
#include "Application/Import/MicrosoftVocabulary.h"
#include "Widget/HelpWindow.h"
#include "Widget/VocabularyWidget.h"
#include "Dialog/NewDatabaseDialog.h"
#include "Dialog/NewVocabularyGroupDialog.h"

MainWindow::MainWindow( QWidget *parent ) :
	QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    initIcons();
    createReccentFileActions();
    updateRecentFileActions();
    initDatabase();
}

MainWindow::~MainWindow()
{
	delete wdgVocabulary;
    delete ui;
}

void MainWindow::initWidgets()
{
	wdgVocabulary	= new VocabularyWidget( this );
	setCentralWidget( wdgVocabulary );
}

void MainWindow::initIcons()
{
	setWindowIcon( QIcon( ":/Resources/icons/dictionary.svg" ) );

	//QIcon::setThemeName( "breeze" );
	ui->actionNew_DB->setIcon( QIcon( ":/Resources/icons/document-new.svg" ) );
	ui->actionOpen_DB->setIcon( QIcon( ":/Resources/icons/document-open.svg" ) );
	ui->menu_Open_Database_Recent->setIcon( QIcon( ":/Resources/icons/document-open-recent.svg" ) );

	ui->menuImport->setIcon( QIcon( ":/Resources/icons/document-import.svg" ) );
	ui->actionInsertGroup->setIcon( QIcon( ":/Resources/icons/folder-new.svg" ) );
	ui->actionInsertWord->setIcon( QIcon( ":/Resources/icons/mail-message-new.svg" ) );

	ui->actionHelp->setIcon( QIcon( ":/Resources/icons/system-help.svg" ) );
	ui->actionAbout->setIcon( QIcon( ":/Resources/icons/help-about.svg" ) );
}

void MainWindow::on_actionInsertGroup_triggered()
{
	NewVocabularyGroupDialog *dlg	= new NewVocabularyGroupDialog( this );

	dlg->setModal( true );
	dlg->show();
}

void MainWindow::on_actionInsertWord_triggered()
{
	wdgVocabulary->insertWord();
}

void MainWindow::on_actionNew_DB_triggered()
{
	createNewDatabase();
}

void MainWindow::on_actionOpen_DB_triggered()
{
	QString dbPath = QFileDialog::getOpenFileName( this, tr( "Open Database" ), QDir::homePath(), tr( "Db Files (*.db)" ) );

	if ( ! dbPath.isEmpty() ) {
		loadDb( dbPath );
	}
}

void MainWindow::on_actionAbout_triggered()
{
	QMessageBox::about(
		this,
		tr( "About QVocabulary" ),
		tr( "The <b>QVocabulary</b> is a vocabulary application "
		   "an alternative of Microsoft Vocabulary++.")

	);
}

void MainWindow::on_actionHelp_triggered()
{
	wdgHelp = new HelpWindow( this );
	wdgHelp->setWindowFlags( Qt::Window );
	//wdgHelp->setModal( true );
	wdgHelp->show();
}

void MainWindow::openRecentFile()
{
    QAction *action = qobject_cast<QAction *>( sender() );
    if ( action ) {
        loadDb( action->data().toString() );
    }
}

void MainWindow::clearRecentFiles()
{
	QStringList emptyList	= QStringList();

    QSettings* settings	= VsSettings::instance()->settings();
    settings->setValue( "recentFileList", emptyList );
    settings->sync();	// Sync ini file

    updateRecentFileActions();
}

void MainWindow::createReccentFileActions()
{
	for ( int i = 0; i < MaxRecentFiles; ++i ) {
		recentFileActs[i] = new QAction( this );
		recentFileActs[i]->setVisible( false );
		connect( recentFileActs[i], SIGNAL( triggered() ),this, SLOT( openRecentFile() ) );
	}

	for ( int i = 0; i < MaxRecentFiles; ++i )
		ui->menu_Open_Database_Recent->addAction( recentFileActs[i] );

	actClearRecentFiles	= new QAction( this );
	actClearRecentFiles->setText( tr( "Clear List" ) );
	actClearRecentFiles->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	actClearRecentFiles->setVisible( false );
	connect( actClearRecentFiles, SIGNAL( triggered() ),this, SLOT( clearRecentFiles() ) );

	separatorAct = ui->menu_Open_Database_Recent->addSeparator();
	ui->menu_Open_Database_Recent->addAction( actClearRecentFiles );
}

void MainWindow::loadDb( const QString &dbPath )
{
	if ( curDatabase == dbPath )
		return;

	if ( ! QFile::exists( dbPath ) ) {
		QMessageBox::warning( this, tr( "Recent Databases" ),
			tr( "Cannot read database: %1." )
			.arg( dbPath )
		);

		return;
	}

	QApplication::setOverrideCursor( Qt::WaitCursor );

	VsDatabase::instance()->connect( dbPath );
	initWidgets();
	wdgVocabulary->initModels();

	QApplication::restoreOverrideCursor();

	setCurrentDb( dbPath );
	statusBar()->showMessage( tr( "Database loaded" ), 2000 );
}

void MainWindow::setCurrentDb( const QString &dbPath )
{
    curDatabase = dbPath;
    setWindowFilePath( curDatabase );

    QSettings* settings	= VsSettings::instance()->settings();
    QStringList files = settings->value( "recentFileList" ).toStringList();
    files.removeAll( dbPath );
    files.prepend( dbPath );
    while ( files.size() > MaxRecentFiles )
        files.removeLast();

    settings->setValue( "recentFileList", files );
    settings->sync();	// Sync ini file

    updateRecentFileActions();
}

void MainWindow::updateRecentFileActions()
{
	QSettings* settings	= VsSettings::instance()->settings();
    QStringList files = settings->value( "recentFileList" ).toStringList();

    int numRecentFiles = qMin( files.size(), (int)MaxRecentFiles );

    for ( int i = 0; i < numRecentFiles; ++i ) {
        QString text = tr( "&%1 %2").arg( i + 1 ).arg( strippedName( files[i] ) );
        recentFileActs[i]->setText( text );
        recentFileActs[i]->setData( files[i] );
        recentFileActs[i]->setVisible( true );
    }

    for ( int j = numRecentFiles; j < MaxRecentFiles; ++j )
        recentFileActs[j]->setVisible( false );

    ui->actionEmpty->setVisible( numRecentFiles == 0 );
    separatorAct->setVisible( numRecentFiles > 0 );
    actClearRecentFiles->setVisible( numRecentFiles > 0 );
}

QString MainWindow::strippedName( const QString &fullDbPath )
{
	QFileInfo fi( fullDbPath );

    return "..." + fi.absolutePath().right( 20 ) + "/" + fi.fileName();
}

void MainWindow::initDatabase()
{
	QString dbPath;
	QSettings* settings	= VsSettings::instance()->settings();
	QStringList files = settings->value( "recentFileList" ).toStringList();

	if ( files.size() && QFile::exists( files.at( 0 ) ) ) {
		dbPath	= files.at( 0 );
	} else {
		createNewDatabase();
	}

	loadDb( dbPath );
}

void MainWindow::createNewDatabase()
{
	dlgNewDatabase	= new NewDatabaseDialog( this );
	dlgNewDatabase->setModal( true );
	dlgNewDatabase->show();
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
