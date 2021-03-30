#include "QuizParametersWidget.h"
#include "ui_QuizParametersWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QIntValidator>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "GlobalTypes.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"
#include "Application/VsSettings.h"

QuizParametersWidget::QuizParametersWidget( QWidget *parent ) :
	QWidget( parent ),
    ui( new Ui::QuizParametersWidget )
{
    ui->setupUi( this );
    quizSettings		= VsSettings::instance()->quizSettings();
    bool displayTimer	= quizSettings["displayQuizAnswerStatus"].toBool();

    ui->chkRandomize->setChecked( quizSettings["randomiizeWords"].toBool() );
    ui->chkTimer->setChecked( displayTimer );

    initGroups();
    initTimer( displayTimer );
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

	connect( ui->chkTimer, SIGNAL( clicked( bool ) ), this, SLOT( initTimer( bool ) ) );
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

void QuizParametersWidget::initGroups()
{
	qx::QxModel<VocabularyGroup>* pModelVocabularyGroup	= new qx::QxModel<VocabularyGroup>();
	pModelVocabularyGroup->qxFetchAll();

	QVBoxLayout *vbox = new QVBoxLayout;
	for ( int i = 0; i < pModelVocabularyGroup->rowCount(); ++i ) {
		int groupId		= pModelVocabularyGroup->data( pModelVocabularyGroup->index( i, 0 ) ).toInt();
		QString groupName	= pModelVocabularyGroup->data( pModelVocabularyGroup->index( i, 1 ) ).toString();

		QCheckBox* chk		= new QCheckBox( groupName, ui->grpGroups );
		chk->setProperty( "groupId", QVariant( groupId ) );
		chk->setProperty( "groupName", groupName );

		chkGroups.append( chk );
		connect( ui->checkAll, SIGNAL ( toggled( bool ) ), chk, SLOT ( setChecked( bool ) ) );

		vbox->addWidget( chk );
	}
	ui->grpGroups->setLayout( vbox );
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

void QuizParametersWidget::changeEvent( QEvent* event )
{
    if ( event->type() == QEvent::LanguageChange )
    {
        ui->retranslateUi( this );
    }

    // remember to call base class implementation
    QWidget::changeEvent( event );
}
