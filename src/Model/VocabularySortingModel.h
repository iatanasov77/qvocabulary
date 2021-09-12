#ifndef MODEL_VOCABULARYSORTING_H
#define MODEL_VOCABULARYSORTING_H

#include <QSortFilterProxyModel>

class VocabularySortingModel : public QSortFilterProxyModel
{
    Q_OBJECT

	public:
		VocabularySortingModel( QObject *parent = 0 );

		void clearFilter();
		void clearSorting();
		void setFilterWordType( int type );

	protected:
		bool lessThan( const QModelIndex &left, const QModelIndex &right ) const override;
		bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;

	private:
		int wordType;

		QList<int> getTranslationsTypes( int wordId ) const;
};

#endif	// MODEL_VOCABULARYSORTING_H
