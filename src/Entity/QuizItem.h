#ifndef ENTITY_QUIZITEM_H
#define ENTITY_QUIZITEM_H

#include "precompiled.h"
#include "Quiz.h"

class QuizItem
{
	public:
		QuizItem();
		virtual ~QuizItem();

		long id;
		long quiz_id;	// To can Move Groups( I dont know how to do this with related object)
		//QuizPtr quiz;

		QString language_1;
		QString language_2;
		QString answer;
		bool rightAnswer;
};

QX_REGISTER_HPP_QX_VOCABULARY(QuizItem, qx::trait::no_base_class_defined, 0)

typedef QSharedPointer<QuizItem> QuizItemPtr;
typedef QVector<QuizItemPtr> ListQuizItem;

#endif // ENTITY_QUIZITEM_H
