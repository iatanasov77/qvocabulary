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
#include "Dialog/InitDatabaseDialog.h"
#include "Dialog/NewDatabaseDialog.h"
#include "Dialog/NewVocabularyGroupDialog.h"

MainWindow::MainWindow( QWidget *parent ) :
	QMainWindow( parent ),
    ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    initIcons();
    createReccentDatabaseActions();
    updateRecentDatabaseActions();
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
	QString dbPath = openDatabase();

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

void MainWindow::openRecentDatabase()
{
    QAction *action = qobject_cast<QAction *>( sender() );
    if ( action ) {
        loadDb( action->data().toString() );
    }
}

void MainWindow::clearRecentDatabases()
{
	QStringList emptyList	= QStringList();

    QSettings* settings	= VsSettings::instance()->settings();
    settings->setValue( "recentDatabaseList", emptyList );
    settings->sync();	// Sync ini file

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
	actClearRecentDatabases->setText( tr( "Clear List" ) );
	actClearRecentDatabases->setIcon( QIcon( ":/Resources/icons/amarok_cart_remove.svg" ) );
	actClearRecentDatabases->setVisible( false );
	connect( actClearRecentDatabases, SIGNAL( triggered() ),this, SLOT( clearRecentDatabases() ) );

	separatorAct = ui->menu_Open_Database_Recent->addSeparator();
	ui->menu_Open_Database_Recent->addAction( actClearRecentDatabases );
}

void MainWindow::updateRecentDatabaseActions()
{
	QSettings* settings		= VsSettings::instance()->settings();
    QStringList databases	= settings->value( "recentDatabaseList" ).toStringList();

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
}

QString MainWindow::strippedName( const QString &fullDbPath )
{
	QFileInfo fi( fullDbPath );

    return "..." + fi.absolutePath().right( 20 ) + "/" + fi.fileName();
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

    QSettings* settings		= VsSettings::instance()->settings();
    QStringList databases	= settings->value( "recentDatabaseList" ).toStringList();
    databases.removeAll( dbPath );
    databases.prepend( dbPath );
    while ( databases.size() > MaxRecentDatabases )
        databases.removeLast();

    settings->setValue( "recentDatabaseList", databases );
    settings->sync();	// Sync ini file

    updateRecentDatabaseActions();
}

void MainWindow::initDatabase()
{
	QString dbPath;
	QSettings* settings	= VsSettings::instance()->settings();
	QStringList files = settings->value( "recentDatabaseList" ).toStringList();

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
			}
		}
	}

	loadDb( dbPath );
}

QString MainWindow::openDatabase()
{
	return QFileDialog::getOpenFileName( this, tr( "Open Database" ), QDir::homePath(), tr( "Db Files (*.db)" ) );
}

QString MainWindow::createNewDatabase()
{
	dlgNewDatabase	= new NewDatabaseDialog( this );
	dlgNewDatabase->setModal( true );
	if ( dlgNewDatabase->exec() == QDialog::Accepted )
		return dlgNewDatabase->database();
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
