/****************************************************************
**
** QVocabulary
**
****************************************************************/
#include <QApplication>
#include "MainWindow.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    MainWindow mw;
    mw.setAcceptDrops( true );
    mw.show();

    //qDebug() << "Icon Theme Search Paths: " << QIcon::themeSearchPaths();
    return app.exec();
}
