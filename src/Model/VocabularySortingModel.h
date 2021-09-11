#ifndef MODEL_VOCABULARYSORTING_H
#define MODEL_VOCABULARYSORTING_H

#include <QSortFilterProxyModel>

class VocabularySortingModel : public QSortFilterProxyModel
{
    Q_OBJECT

	public:
		VocabularySortingModel( QObject *parent = 0 );

	protected:
		bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;

};

#endif	// MODEL_VOCABULARYSORTING_H
