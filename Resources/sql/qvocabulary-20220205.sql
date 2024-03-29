create table VocabularyMetaInfo (
	id integer primary key, 
	name varchar(255), 
	language1 varchar(255), 
	language2 varchar(255),
	db_version varchar(16)
);

create table Vocabulary (
	id integer primary key, 
	language_1 varchar(255),
	transcription varchar(255),
	language_2 varchar(255), 
	group_id integer,
	description varchar(255)
);

create table VocabularyGroup (
	id integer primary key, 
	name varchar(255)
);

create table Quiz (
	id integer primary key,
	direction INTEGER,
	randomize BOOLEAN,
	from_vocabulary INTEGER,
	groups TEXT,
	assessment INTEGER,
	started_at DATETIME,
	finished_at DATETIME
);

create table QuizItem (
	id integer primary key,
	quiz_id integer,
	language_1 varchar(255),
	transcription varchar(255),
	language_2 varchar(255), 
	answer varchar(255),
	right_answer BOOLEAN
);

create table ArchiveWord (
	id integer primary key, 
	language_1 varchar(255),
	transcription varchar(255),
	language_2 varchar(255), 
	group_id integer,
	description varchar(255)
);

create table ArchiveGroup (
	id integer primary key, 
	name varchar(255)
);

create table VocabularyWordSynonym (
	id INTEGER UNIQUE NOT NULL PRIMARY KEY, 	
	word_id INTEGER NOT NULL,
  	synonym_id INTEGER,
  	only_words VARCHAR(255),
  	target VARCHAR(64) NOT NULL
);

create table VocabularyWordTranslation ( 	
	word_id INTEGER NOT NULL,
  	tr_word VARCHAR(255) NOT NULL,
  	tr_type VARCHAR(255),
  	tr_weight INTEGER,
  	PRIMARY KEY (word_id, tr_word)
);

