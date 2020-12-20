create table VocabularyMetaInfo (
	id integer primary key, 
	name varchar(255), 
	language1 varchar(255), 
	language2 varchar(255)
);

create table Vocabulary (
	id integer primary key, 
	language_1 varchar(255), 
	language_2 varchar(255), 
	group_id integer
);

create table VocabularyGroup (
	id integer primary key, 
	name varchar(255)
);

create table Quiz (
	id integer primary key,
	direction INTEGER,
	randomize BOOLEAN,
	groups TEXT,
	started_at DATETIME
);

create table QuizItem (
	id integer primary key,
	quiz_id integer,
	language_1 varchar(255), 
	language_2 varchar(255), 
	answer varchar(255),
	right_answer BOOLEAN
);
