#include "MultiSelectComboBoxDelegate.h"

#include <QApplication>
#include <QCheckBox>
#include <QDebug>

MultiSelectComboBoxDelegate::MultiSelectComboBoxDelegate( QObject *parent ) : QItemDelegate( parent )
{

}

void MultiSelectComboBoxDelegate::paint( QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	//Get item data
	bool value = index.data( Qt::CheckStateRole ).toBool();
	QString text = index.data( Qt::DisplayRole ).toString();

	// fill style options with item data
	const QStyle *style = QApplication::style();
	QStyleOptionButton opt;
	opt.state |= value ? QStyle::State_On : QStyle::State_Off;
	opt.state |= QStyle::State_Enabled;
	opt.text = text;
	opt.rect = option.rect;

	// draw item data as CheckBox
	style->drawControl( QStyle::CE_CheckBox, &opt, painter );
	//QMessageBox::information(0,"Info",text);

	qDebug() << "MultiSelectComboBoxDelegate::paint";
}

QWidget* MultiSelectComboBoxDelegate::createEditor( QWidget *parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	// create check box as our editor
	 QCheckBox *editor = new QCheckBox( parent );

	 return editor;
}

void MultiSelectComboBoxDelegate::setEditorData( QWidget *editor, const QModelIndex &index ) const
{
	//set editor data
	QCheckBox *myEditor = static_cast<QCheckBox*>( editor );
	myEditor->setText( index.data( Qt::DisplayRole ).toString() );
	myEditor->setChecked( index.data( Qt::CheckStateRole ).toBool() );

	qDebug() << "MultiSelectComboBoxDelegate::setEditorData";
}

void MultiSelectComboBoxDelegate::setModelData( QWidget *editor, QAbstractItemModel *model, const QModelIndex &index ) const
{
	//get the value from the editor (CheckBox)
	QCheckBox *myEditor = static_cast<QCheckBox*>( editor );
	bool value = myEditor->isChecked();

	//set model data
	QMap<int,QVariant> data;
	data.insert( Qt::DisplayRole, myEditor->text() );
	data.insert( Qt::CheckStateRole, value );
	model->setItemData( index, data );

	qDebug() << "MultiSelectComboBoxDelegate::setModelData";
}

void MultiSelectComboBoxDelegate::updateEditorGeometry( QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index ) const
{
	editor->setGeometry( option.rect );
}
