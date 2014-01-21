#include "mainwindow.h"
#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QStyle>

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    MainWin win;

    QRect
        screen_geometry     = QApplication::desktop()->screenGeometry();
    int
        width               = (int)( screen_geometry.width()  * 0.9 ),
        height              = (int)( screen_geometry.height() * 0.9 ),
        x                   = ( screen_geometry.width()  - width  ) / 2,
        y                   = ( screen_geometry.height() - height ) / 2;
    win.setFixedSize( width, height );
    win.move( x, y );
    win.setWindowTitle( "Welcome to ArchiTech SDK" );
    win.show();

    QWebSettings::enablePersistentStorage();

    return app.exec();
}
