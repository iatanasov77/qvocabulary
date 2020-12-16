#include "QuizParametersWidget.h"
#include "ui_QuizParametersWidget.h"

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/VocabularyGroup.h"

QuizParametersWidget::QuizParametersWidget( QWidget *parent ) :
	QWidget( parent ),
    ui( new Ui::QuizParametersWidget )
{
    ui->setupUi( this );

    initGroups();
}

QuizParametersWidget::~QuizParametersWidget()
{
    delete ui;
}

void QuizParametersWidget::initGroups()
{
	qx::QxModel<VocabularyGroup>* pModelVocabularyGroup	= new qx::QxModel<VocabularyGroup>();
	pModelVocabularyGroup->qxFetchAll();

	QVBoxLayout *vbox = new QVBoxLayout;
	for ( int i = 0; i < pModelVocabularyGroup->rowCount(); ++i ) {
		QString groupName	= pModelVocabularyGroup->data( pModelVocabularyGroup->index( i, 1 ) ).toString();
		QCheckBox* chk		= new QCheckBox( groupName, this );

		connect( ui->checkAll, SIGNAL ( toggled( bool ) ), chk, SLOT ( setChecked( bool ) ) );

		vbox->addWidget( chk );
	}
	ui->groupBox_2->setLayout( vbox );
}

int QuizParametersWidget::initDatabase()
{
	if ( ui->rbFirst2Second->isChecked() ) {
		return NewDatabase;
	} else if ( ui->rbSecond2First->isChecked() ) {
		return OpenDatabase;
	}
}

QPushButton* QuizParametersWidget::btnStartQuiz()
{
	return ui->btnStartQuiz;
}
