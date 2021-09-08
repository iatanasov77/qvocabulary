/*
 * This Class is used to import another QVocabulary Database
 * -------------------------------------------------------------
 * This needed when a new database structure is needed
 * to can import old data from old Database.
 */
#include "QVocabulary.h"

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Application/VsDatabase.h"
#include "Entity/VocabularyMetaInfo.h"
#include "Entity/VocabularyGroup.h"
#include "Entity/Vocabulary.h"

#include "Entity/Quiz.h"
#include "Entity/QuizItem.h"
#include "Entity/ArchiveGroup.h"
#include "Entity/ArchiveWord.h"
#include "Entity/VocabularyWordSynonym.h"
#include "Entity/VocabularyWordTranslation.h"

QSqlDatabase QVocabulary::db;

bool QVocabulary::importFromDb( QString dbName, bool importQuizes, bool importArchive )
{
	qDebug() << "Import DB: " << dbName;
	db	= QSqlDatabase::addDatabase( "QSQLITE", "import_source" );
	db.setDatabaseName( dbName );
	db.setHostName( "localhost" );
	db.setUserName( "root" );
	db.setPassword( "root" );

	qDebug() << "Import Started.";
	if ( db.open() ) {
		qDebug() << "DB is Opened.";
		parseMeta();
		parseWords();
		_importSynonyms();
		_importTranslationTypes();

		if ( importQuizes ) {
			_importQuizes();
		}

		if ( importArchive ) {
			_importArchive();
		}

		db.close();
	} else {
		qDebug() << "Import Source Database Cannot Be Opened!";
	}

	return true;
}

bool QVocabulary::parseMeta()
{
	QSqlQuery query( "SELECT * FROM VocabularyMetaInfo", QVocabulary::db );
	query.next();

	// Update VocabularyMetaInfo
	VocabularyMetaInfoPtr metaInfo	= VsDatabase::instance()->metaInfo();
	metaInfo->name			= query.value( "name" ).toString();
	metaInfo->language1		= query.value( "language1" ).toString();
	metaInfo->language2		= query.value( "language2" ).toString();

	QSqlError daoError	= qx::dao::update( metaInfo );

	return true;
}

bool QVocabulary::parseWords()
{
	QSqlError daoError;
	VocabularyPtr voc;
	VocabularyGroupPtr vocg;
	QMap<int, int> importedGroups;
	int currentGroup	= -1;
	bool importWord		= false;

	QSqlQuery query( "SELECT * FROM Vocabulary", db );
	while ( query.next() ) {
		currentGroup	= query.value( "group_id" ).toInt();
		if ( ! importedGroups.contains( currentGroup ) ) {
			QSqlQuery queryGroup( QString( "SELECT * FROM VocabularyGroup WHERE id = %1" ).arg( currentGroup ), db );
			queryGroup.next();

			QString groupName	= queryGroup.value( "name" ).toString();
			// Some Fix for Empty Group Names but I dont know if this is the right in Import Functionality
			if ( groupName.length() ) {
				// Group Name Prefix get from Settings: Imported
				vocg	= createGroup( importSettings["groupPrefix"].toString() + queryGroup.value( "name" ).toString() );
				importedGroups[currentGroup]	= vocg->id;

				importWord		= true;
			} else {
				importWord		= false;
			}

		}

		if ( importWord ) {
			voc				= VocabularyPtr( new Vocabulary() );

			voc->group_id		= importedGroups[currentGroup];	// To can Move Groups( I dont know how to do this with related object)

			voc->language_1		= query.value( "language_1" ).toString();
			voc->transcription	= query.value( "transcription" ).toString();
			voc->language_2		= query.value( "language_2" ).toString();
			voc->description	= query.value( "description" ).toString();

			daoError			= qx::dao::insert( voc );
		}
	}

	return true;
}

bool QVocabulary::_importQuizes()
{
	QSqlError daoError;
	QuizPtr quiz;
	QuizItemPtr quizItem;
	QString strQuizItemQuery;

	QSqlQuery quizQuery( "SELECT * FROM Quiz", db );
	while ( quizQuery.next() ) {
		quiz				= QuizPtr( new Quiz() );
		quiz->direction		= quizQuery.value( "direction" ).value<EnumDirection>();
		quiz->randomize		= quizQuery.value( "randomize" ).toBool();
		quiz->groups		= quizQuery.value( "groups" ).toString();
		quiz->assessment	= quizQuery.value( "assessment" ).toInt();
		quiz->startedAt		= quizQuery.value( "started_at" ).toDateTime();
		quiz->finishedAt	= quizQuery.value( "finished_at" ).toDateTime();
		daoError			= qx::dao::insert( quiz );

		strQuizItemQuery	= QString( "SELECT * FROM QuizItem WHERE quiz_id=%1" ).arg( QString::number( quizQuery.value( "id" ).toInt() ) );
		QSqlQuery quizItemQuery( strQuizItemQuery, db );
		while ( quizItemQuery.next() ) {
			quizItem				= QuizItemPtr( new QuizItem() );
			quizItem->quiz_id		= quiz->id;
			quizItem->language_1	= quizItemQuery.value( "language_1" ).toString();
			quizItem->language_2	= quizItemQuery.value( "language_2" ).toString();
			quizItem->answer		= quizItemQuery.value( "answer" ).toString();
			quizItem->rightAnswer	= quizItemQuery.value( "right_answer" ).toBool();
			daoError				= qx::dao::insert( quizItem );
		}
	}

	return true;
}

bool QVocabulary::_importArchive()
{
	QSqlError daoError;
	ArchiveGroupPtr archiveGroup;
	ArchiveWordPtr archiveWord;
	QString strArchiveWordQuery;

	QSqlQuery archiveGroupQuery( "SELECT * FROM ArchiveGroup", db );
	while ( archiveGroupQuery.next() ) {
		archiveGroup		= ArchiveGroupPtr( new ArchiveGroup() );
		archiveGroup->name	= archiveGroupQuery.value( "name" ).toString();
		daoError			= qx::dao::insert( archiveGroup );

		strArchiveWordQuery	= QString( "SELECT * FROM ArchiveWord WHERE group_id=%1" ).arg( QString::number( archiveGroupQuery.value( "id" ).toInt() ) );
		QSqlQuery archiveWordQuery( strArchiveWordQuery, db );
		while ( archiveWordQuery.next() ) {
			archiveWord					= ArchiveWordPtr( new ArchiveWord() );
			archiveWord->group_id		= archiveGroup->id;
			archiveWord->language_1		= archiveWordQuery.value( "language_1" ).toString();
			archiveWord->language_2		= archiveWordQuery.value( "language_2" ).toString();
			archiveWord->transcription	= archiveWordQuery.value( "transcription" ).toString();
			archiveWord->description	= archiveWordQuery.value( "description" ).toString();
			daoError					= qx::dao::insert( archiveWord );
		}
	}

	return true;
}

bool QVocabulary::_importSynonyms()
{
	QSqlError daoError;
	VocabularyWordSynonymPtr synonym;

	QSqlQuery synonymQuery( "SELECT * FROM VocabularyWordSynonym", db );
	while ( synonymQuery.next() ) {
		synonym	= VocabularyWordSynonymPtr( new VocabularyWordSynonym() );
		synonym->word_id	= synonymQuery.value( "word_id" ).toInt();
		synonym->synonym_id	= synonymQuery.value( "synonym_id" ).toInt();
		synonym->only_words	= synonymQuery.value( "only_words" ).toString();
		synonym->target		= synonymQuery.value( "target" ).toString();

		daoError			= qx::dao::insert( synonym );
	}

	return true;
}

bool QVocabulary::_importTranslationTypes()
{

}
