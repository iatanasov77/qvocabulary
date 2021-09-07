#include "QuizParametersWidget.h"
#include "ui_QuizParametersWidget.h"

#include <QWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/ArchiveGroup.h"
#include "Application/QVocabularySettings.h"

QuizParametersWidget::QuizParametersWidget( QWidget *parent ) :
	QWidget( parent ),
    ui( new Ui::QuizParametersWidget )
{
    ui->setupUi( this );
    // Hide GroupBox Title
    ui->grpGroups->setStyleSheet( "QGroupBox{padding-top:15px; margin-top:-15px; border:0;}" );

    quizSettings		= QVocabularySettings::instance()->quizSettings();
    bool displayTimer	= quizSettings["displayTimer"].toBool();

    ui->chkDisplayTranscription->setChecked( quizSettings["displayTranscriptions"].toBool() );
    ui->chkRandomize->setChecked( quizSettings["randomizeWords"].toBool() );
    ui->chkTimer->setChecked( displayTimer );

    initGroups();
    initTimer( displayTimer );
    initWordCount( false );
    setDirection();

    connect(
		ui->rbFirst2Second,
		SIGNAL( clicked() ),
		this,
		SLOT( setDirection() )
	);

	connect(
		ui->rbSecond2First,
		SIGNAL( clicked() ),
		this,
		SLOT( setDirection() )
	);

	connect(
		ui->rbVocabulary,
		SIGNAL( clicked() ),
		this,
		SLOT( setGroups() )
	);

	connect(
		ui->rbArchive,
		SIGNAL( clicked() ),
		this,
		SLOT( setGroups() )
	);

	connect( ui->chkTimer, SIGNAL( clicked( bool ) ), this, SLOT( initTimer( bool ) ) );
	connect( ui->chkWordCount, SIGNAL( clicked( bool ) ), this, SLOT( initWordCount( bool ) ) );
}

QuizParametersWidget::~QuizParametersWidget()
{
    delete ui;
}

void QuizParametersWidget::setMetaInfo( VocabularyMetaInfoPtr metaInfo )
{
	QString lblFirst2Second = QString( "%1 to %2" ).arg( metaInfo->language1 ).arg( metaInfo->language2 );
	QString lblSecond2First = QString( "%1 to %2" ).arg( metaInfo->language2 ).arg( metaInfo->language1 );

	ui->rbFirst2Second->setText( qApp->translate( "QuizParametersWidget", lblFirst2Second.toStdString().c_str() ) );
	ui->rbSecond2First->setText( qApp->translate( "QuizParametersWidget", lblSecond2First.toStdString().c_str() ) );
}

void QuizParametersWidget::initTimer( bool on )
{
	QTime timer( 0, 0 );
	ui->teTimer->setTime( timer.addSecs( quizSettings["timerDefaultTime"].toInt() ) );

	if ( on ) {
		ui->frmTime->show();
	} else {
		ui->frmTime->hide();
	}
}

void QuizParametersWidget::initWordCount( bool on )
{
	if ( on ) {
		ui->frmWordCount->show();
	} else {
		ui->frmWordCount->hide();
	}
}

void QuizParametersWidget::initGroups()
{
	QVBoxLayout *box = new QVBoxLayout;
	setVocabularyGroups( box );
	ui->grpGroups->setLayout( box );
}

void QuizParametersWidget::clearGroups( QLayout *box )
{
	while( ! box->isEmpty() ) {
		QWidget *w = box->takeAt( 0 )->widget();
		delete w;
	}
}

void QuizParametersWidget::setGroups()
{
	QLayout *box	= ui->grpGroups->layout();

	clearGroups( box );
	chkGroups.clear();
	if ( ui->rbVocabulary->isChecked() ) {
		setVocabularyGroups( box );
	} else if ( ui->rbArchive->isChecked() ) {
		setArchiveGroups( box );
	}
}

void QuizParametersWidget::setVocabularyGroups( QLayout *box )
{
	qx::QxModel<VocabularyGroup>* pModelGroup	= new qx::QxModel<VocabularyGroup>();
	pModelGroup->qxFetchAll();

	for ( int i = 0; i < pModelGroup->rowCount(); ++i ) {
		int groupId			= pModelGroup->data( pModelGroup->index( i, 0 ) ).toInt();
		QString groupName	= pModelGroup->data( pModelGroup->index( i, 1 ) ).toString();
		//qDebug() << "Added Group: " << groupName;

		QCheckBox* chk		= new QCheckBox( groupName, ui->grpGroups );
		chk->setProperty( "groupId", QVariant( groupId ) );
		chk->setProperty( "groupName", groupName );

		chkGroups.append( chk );
		connect( ui->checkAll, SIGNAL ( toggled( bool ) ), chk, SLOT ( setChecked( bool ) ) );

		box->addWidget( chk );
	}
}

void QuizParametersWidget::setArchiveGroups( QLayout *box )
{
	qx::QxModel<ArchiveGroup>* pModelGroup	= new qx::QxModel<ArchiveGroup>();
	pModelGroup->qxFetchAll();

	for ( int i = 0; i < pModelGroup->rowCount(); ++i ) {
		int groupId			= pModelGroup->data( pModelGroup->index( i, 0 ) ).toInt();
		QString groupName	= pModelGroup->data( pModelGroup->index( i, 1 ) ).toString();
		//qDebug() << "Added Group: " << groupName;

		QCheckBox* chk		= new QCheckBox( groupName, ui->grpGroups );
		chk->setProperty( "groupId", QVariant( groupId ) );
		chk->setProperty( "groupName", groupName );

		chkGroups.append( chk );
		connect( ui->checkAll, SIGNAL ( toggled( bool ) ), chk, SLOT ( setChecked( bool ) ) );

		box->addWidget( chk );
	}
}

void QuizParametersWidget::setDirection()
{
	if ( ui->rbFirst2Second->isChecked() ) {
		direction	= FIRST_TO_SECOND;
	} else if ( ui->rbSecond2First->isChecked() ) {
		direction	= SECOND_TO_FIRST;
	}
}

QPushButton* QuizParametersWidget::btnStartQuiz()
{
	return ui->btnStartQuiz;
}

QList<QCheckBox*> QuizParametersWidget::getChkGroups()
{
	return chkGroups;
}

EnumDirection QuizParametersWidget::getDirection()
{
	return direction;
}

QCheckBox* QuizParametersWidget::getChkDisplayTranscriptions()
{
	return ui->chkDisplayTranscription;
}

QCheckBox* QuizParametersWidget::getChkRandomize()
{
	return ui->chkRandomize;
}

int QuizParametersWidget::time()
{
	int time	= 0;
	if ( ui->chkTimer->isChecked() ) {
		QTime tdt	= ui->teTimer->time();
		time		= ( tdt.hour() * 3600 ) + ( tdt.minute() * 60 ) + tdt.second();
	}

	return time;
}

int QuizParametersWidget::wordsCount()
{
	if ( ui->chkWordCount->isChecked() ) {
		return ui->spWordCount->value();
	}

	return 0;
}

QString QuizParametersWidget::wordsFrom()
{
	if ( ui->rbArchive->isChecked() ) {
		return "ArchiveWord";
	}

	return "Vocabulary";
}

void QuizParametersWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
