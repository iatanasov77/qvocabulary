#include "VocabularySynonymsDelegate.h"

#include <QApplication>
#include <QLineEdit>
#include <QIcon>
#include <QMouseEvent>
#include <QDebug>
#include <QPainter>
#include <QTextDocument>

#include "ModelView/VocabularyTableView.h"
#include "../lib/VankoSoft/Widget/VsClickableLabel.h"
#include "../lib/VankoSoft/Widget/VsIconLabel.h"

static int countWords;
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

	if ( index.column() == 6 ) {
		QStyledItemDelegate::paint( painter, option, QModelIndex() );	// Called parent with empty Index
																		// to draw the selection background only
		painter->save();

		// Button
		QStyleOptionButton button	= createButton( buttonRect( option.rect ) );
		QStyle *style				= option.widget ? option.widget->style() : QApplication::style();
		style->drawControl( QStyle::CE_PushButton, &button, painter, option.widget );

		// Links to Synonyms
		QString text			= index.data( Qt::DisplayRole ).toString();
		QList<QVariant> wordIds	= index.data( Qt::UserRole ).toList();
		QStringList words		= text.split( "," );
		countWords				= words.size();
		QStyleOptionViewItem op( option );

		//qDebug() << "COUNT WORD IDS: " << wordIds.size();

		int wordId;
		int wordNumber	= 1;
		QMap<int, QRect> rowWordRects;
		foreach( QString word, words ) {
			word	= word.trimmed();

			if ( word.size( ) ) {
				wordId					= wordNumber >= wordIds.size() ? 0 : wordIds[wordNumber - 1].toInt();
				op.rect					= textRect( option.rect, wordNumber );
				rowWordRects[wordId]	= op.rect;
				initStyleOption( &op, index );
				createWord( painter, op, index, word, wordId );
				wordNumber++;
			}
		}
		VocabularySynonymsDelegate::wordRects[index.row()]	= rowWordRects;

		painter->restore();
	} else {
		QStyledItemDelegate::paint( painter, option, index );
	}
}

bool VocabularySynonymsDelegate::editorEvent(
	QEvent *event,
	QAbstractItemModel *model,
	const QStyleOptionViewItem &option,
	const QModelIndex &index
) {
	if ( event->type() == QEvent::MouseButtonRelease ) {
		QMouseEvent* e	= ( QMouseEvent* )event;
		int clickX		= e->x();
		int clickY		= e->y();

		QRect btnRect	= buttonRect( option.rect );
		if(
			( clickX > btnRect.x() && clickX < btnRect.x() + btnRect.width() ) &&
			( clickY > btnRect.y() && clickY < btnRect.y() + btnRect.height() )
		) {
			emit buttonClicked( index );
		}

		QRect wordRect;
		foreach ( int wordId, VocabularySynonymsDelegate::wordRects[index.row()].keys() ) {
			wordRect	= wordRects[index.row()][wordId];
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

void VocabularySynonymsDelegate::createWord( QPainter *painter, QStyleOptionViewItem op, QModelIndex index, QString text, int wordId ) const
{
	Q_UNUSED( index );

	//	VsClickableLabel *widget = new VsClickableLabel();
	//	widget->setText(  text );


	VsIconLabel *widget = new VsIconLabel( ":/Resources/icons/dictionary.svg", text );
	widget->setProperty( "labelData", wordId );
	widget->setCursor( QCursor( Qt::PointingHandCursor ) );
	widget->setStyleSheet( "QLabel { border: 1px solid gray; border-radius: 2px; background-color: white; padding: 0px 5px 10px 15px; margin: 20px; }");

	op.widget	= widget;
	op.icon 	= QIcon( ":/Resources/icons/dictionary.svg" );
	op.text		= text;
	op.font.setBold( true );
	op.font.setUnderline( true );

	QStyle *style	= widget ? widget->style() : QApplication::style();
	style->drawControl( QStyle::CE_ItemViewItem, &op, painter, widget );


//	QStyleOptionButton wordItem;
//	wordItem.icon 	= QIcon( ":/Resources/icons/dictionary.svg" );
//	wordItem.text	= text;
//	QStyle *style	= op.widget ? op.widget->style() : QApplication::style();
//	style->drawControl( QStyle::CE_PushButton, &wordItem, painter, op.widget );
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
	if ( countWords ) {
		w	= w / countWords;
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
