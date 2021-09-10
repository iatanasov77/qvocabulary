#ifndef MODEL_QUIZITEM_H
#define MODEL_QUIZITEM_H

#include <QObject>

#include "precompiled.h"
#include "QxOrm_Impl.h"
#include "QxModelView.h"

#include "Entity/QuizItem.h"

class QuizItemModel : public qx::QxModel<QuizItem>
{
	Q_OBJECT

	public:
		QuizItemModel( QObject* parent = nullptr );
	    virtual ~QuizItemModel();

		virtual Qt::ItemFlags flags( const QModelIndex & index ) const;
		void setAnswerIcon( const QModelIndex& index, bool right );
};


#endif	// MODEL_QUIZITEM_H
