#include "VocabularySynonymsDelegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QTextDocument>

#include "GlobalTypes.h"
#include "View/VocabularyTableView.h"
#include "../lib/VankoSoft/Widget/VsClickableLabel.h"
#include "../lib/VankoSoft/Widget/VsIconLabel.h"

int VocabularySynonymsDelegate::countWords;
QMap<int, QMap<int, QRect>> VocabularySynonymsDelegate::wordRects;

/*
 * Change Mouse Cursor: https://stackoverflow.com/questions/9131727/how-can-i-change-the-mouse-pointer-when-mouse-over-text-with-qstyleditemdelegate
 */
VocabularySynonymsDelegate::VocabularySynonymsDelegate( QObject *parent ) : QStyledItemDelegate( parent )
{
	view	= parent;
}

void VocabularySynonymsDelegate::paint(
	QPainter *painter,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) const {
	if ( ! index.isValid() || isEmptyLine( index ) )
		QStyledItemDelegate::paint( painter, option, QModelIndex() );	// draw the selection background only

	//VocabularySynonymsDelegate::countWords	= 0;
	//VocabularySynonymsDelegate::wordRects.clear();
	QStyledItemDelegate::paint( painter, option, QModelIndex() );	// Called parent with empty Index
																	// to draw the selection background only
	painter->save();

	// Button
	if ( ! isEmptyLine( index ) ) {
		QStyleOptionButton button	= createButton( buttonRect( option.rect ) );
		QStyle *style				= option.widget ? option.widget->style() : QApplication::style();

		style->drawControl( QStyle::CE_PushButton, &button, painter, option.widget );
	}

	// Words
	if ( ! isEmptyLine( index ) )
		createWidgets( painter, option, index );

	painter->restore();
}

bool VocabularySynonymsDelegate::editorEvent(
	QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
	QRect btnRect;
	QRect wordRect;
	QMouseEvent* e	= ( QMouseEvent* )event;
	int clickX		= e->x();
	int clickY		= e->y();

	if ( event->type() == QEvent::MouseButtonRelease ) {
		btnRect	= buttonRect( option.rect );
		if(
			( clickX > btnRect.x() && clickX < btnRect.x() + btnRect.width() ) &&
			( clickY > btnRect.y() && clickY < btnRect.y() + btnRect.height() )
		) {
			emit buttonClicked( index );
		}
	}

	if ( event->type() == QEvent::MouseButtonDblClick ) {
		//qDebug() << "DELEGATE EVENT WORD IDS: " << VocabularySynonymsDelegate::wordRects[index.row()].keys();

		foreach ( int wordId, VocabularySynonymsDelegate::wordRects[index.row()].keys() ) {
			wordRect	= VocabularySynonymsDelegate::wordRects[index.row()][wordId];
			//qDebug() << "SYNONYM CLICKED: " << wordId;
			if(
				( clickX > wordRect.x() && clickX < wordRect.x() + wordRect.width() ) &&
				( clickY > wordRect.y() && clickY < wordRect.y() + wordRect.height() )
			) {
				//qDebug() << "SYNONYM CLICKED: " << wordId;
				if ( wordId ) {
					emit synonymClicked( wordId );
				}
			}
		}
	}

	return QStyledItemDelegate::editorEvent( event, model, option, index );
}

QWidget* VocabularySynonymsDelegate::createEditor(
		QWidget *parent,
		const QStyleOptionViewItem &option,
		const QModelIndex &index
) const {
	Q_UNUSED( index );

	QRect editorRect	= textRect( option.rect );
	QLineEdit* editor	= new QLineEdit( parent );

	editor->resize( editorRect.width(), editorRect.height() );

	return editor;
}

void VocabularySynonymsDelegate::createWidgets( QPainter *painter, QStyleOptionViewItem option, QModelIndex index ) const
{
	QMap<QString, QVariant> userData		= index.data( Qt::UserRole ).toMap();
	QString text							= index.data( Qt::DisplayRole ).toString();
	QStringList words						= text.split( "," );
	VocabularySynonymsDelegate::countWords	= words.size();

	int wordIdIndex	= 0;
	int wordId		= 0;
	int wordNumber	= 1;

	QMap<int, QRect> rowWordRects;
	QMap<int, QRect> wordRect;
	QList<QVariant> wordIds;
	QList<QVariant> vocabularyWordIds	= userData.value( SynonymTargets["VOCABULARY"] ).toList();
	QList<QVariant> archiveWordIds		= userData.value( SynonymTargets["ARCHIVE"] ).toList();
	//qDebug() << "VOCABULARY WORD IDS: " << vocabularyWordIds;
	//qDebug() << "ARCHIVE WORD IDS: " << archiveWordIds;

	QString wordTarget	= SynonymTargets["VOCABULARY"];
	wordIds				= vocabularyWordIds;
	foreach( QString word, words ) {
		if ( wordTarget == SynonymTargets["VOCABULARY"] && wordIdIndex >= vocabularyWordIds.size() ) {
			wordTarget	= SynonymTargets["ARCHIVE"];
			wordIds		= archiveWordIds;
			wordIdIndex	= 0;
		}
		else if ( wordTarget == SynonymTargets["ARCHIVE"] && wordIdIndex >= archiveWordIds.size() ) {
			wordTarget	= SynonymTargets["ONLY_WORDS"];
		}

		wordId	= (
					( wordTarget == SynonymTargets["ARCHIVE"] ) ||
					( wordTarget == SynonymTargets["ONLY_WORDS"] ) ||
					( wordIdIndex >= wordIds.size() )
				) ? 0 : wordIds[wordIdIndex].toInt();

		word	= word.trimmed();
		//qDebug() << "DELEGATE WORD ID: " << wordId;
		if ( word.size( ) ) {
			wordRect	= createWidget( painter, option, index, word, wordId, wordNumber, wordTarget );
			if ( wordRect.firstKey() )
				rowWordRects.insert( wordRect );

			wordNumber++;
			wordIdIndex++;
		}
	}
	VocabularySynonymsDelegate::wordRects[index.row()]	= rowWordRects;
	//qDebug() << "DELEGATE WORD IDS: " << rowWordRects.keys();
}

QMap<int, QRect> VocabularySynonymsDelegate::createWidget(
	QPainter *painter,
	QStyleOptionViewItem option,
	QModelIndex index,
	QString word,
	int wordId,
	int wordNumber,
	QString wordTarget
) const {
	QMap<int, QRect> rowWordRects;
	QStyleOptionViewItem op( option );

	op.rect					= textRect( option.rect, wordNumber );
	rowWordRects[wordId]	= wordId ? op.rect : QRect();

	initStyleOption( &op, index );

	op.features			|= QStyleOptionViewItem::HasDecoration;
	op.decorationSize	= QSize( 16, 16 );

	if ( wordTarget == SynonymTargets["VOCABULARY"] )
		op.icon 			= QIcon( ":/Resources/icons/dictionary.svg" );
	else if ( wordTarget == SynonymTargets["ARCHIVE"] )
		op.icon 			= QIcon( ":/Resources/icons/archive.svg" );
	else
		op.icon 			= QIcon( ":/Resources/icons/only_word.png" );

	op.text	= word;
	op.font.setBold( true );
	op.font.setUnderline( true );

	QStyle *style	= op.widget ? op.widget->style() : QApplication::style();
	style->drawControl( QStyle::CE_ItemViewItem, &op, painter, op.widget );

	return rowWordRects;
}

QStyleOptionButton VocabularySynonymsDelegate::createButton( QRect buttonRect  ) const
{
	QStyleOptionButton button;

	button.rect		= buttonRect;
	button.icon 	= QIcon( ":/Resources/icons/column-management.svg" );
	button.iconSize	= QSize( 16, 16 );
	button.state 	= QStyle::State_Enabled;
	//button.text		= "LKVBSFKDBLF";

	return button;
}

QRect VocabularySynonymsDelegate::textRect( QRect cellRect, int wordNumber ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x 		= cellRect.left();
	int y 		= cellRect.top();
	int w 		= cellRect.width() - btnSize;
	if ( VocabularySynonymsDelegate::countWords ) {
		w	= w / VocabularySynonymsDelegate::countWords;
		if ( wordNumber > 1 )
			x	= x + ( w * ( wordNumber - 1 ) );
	}
	int h 		= cellRect.height();

	return QRect( x, y, w, h );
}

QRect VocabularySynonymsDelegate::buttonRect( QRect cellRect ) const
{
	int btnSize	= cellRect.height();	// Button rect should be a square

	int x = cellRect.left() + cellRect.width() - btnSize;
	int y = cellRect.top();
	int w = btnSize;
	int h = btnSize;

	return QRect( x, y, w, h );
}

bool VocabularySynonymsDelegate::isEmptyLine( const QModelIndex index ) const
{
	if ( index.siblingAtColumn( 0 ).data().toInt() )
		return false;
	else
		return true;
}
