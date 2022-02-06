#ifndef ENTITY_QUIZ_H
#define ENTITY_QUIZ_H

#include "precompiled.h"
#include "GlobalTypes.h"

class Quiz
{
	public:
		Quiz();
		virtual ~Quiz();

		long id;
		EnumDirection direction;
		bool randomize;
		EnumFromVocabulary fromVocabulary;
		QString groups;	// Use JSON String of the groups list
		int assessment;
		QDateTime startedAt;
		QDateTime finishedAt;
};

QX_REGISTER_HPP_QX_VOCABULARY(Quiz, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<Quiz> QuizPtr;
typedef QVector<QuizPtr> ListQuiz;

#endif // ENTITY_QUIZ_H
