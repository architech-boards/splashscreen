#include "mainwindow.h"
#include "launcher.h"

#include <QWebFrame>
#include <QWebElementCollection>
#include <QNetworkDiskCache>
#include <QDesktopServices>
#include <QAction>
#include <QSslSocket>
#include <QNetworkReply>

/*
 * Default Constructor
 */
//! [MainWindow - constructor]
MainWin::MainWin(QWidget * parent) : QWebView(parent)
{
    m_network = new QNetworkAccessManager(this);
    m_cache = new QNetworkDiskCache(this);
    m_cache->setCacheDirectory(QDesktopServices::TempLocation + "/architechlauncher");
    m_cache->setMaximumCacheSize(1000000); //set the cache to 10megs
    m_network->setCache(m_cache);

    //Disable context menu
    setContextMenuPolicy(Qt::NoContextMenu);

    //! The object we will expose to JavaScript engine:
    m_launcher = new Launcher(this,(QWebView*)this);

    // Signal is emitted before frame loads any web content:
    QObject::connect(
                        page()->mainFrame(),
                        SIGNAL( javaScriptWindowObjectCleared() ),
                        this,
                        SLOT( addJSObject() )
                    );

    QWebSettings::globalSettings()->setAttribute( QWebSettings::DeveloperExtrasEnabled, true );
    m_launcher->go_home();
}
//! [MainWindow - constructor]

//! [MainWindow - addJSObject]
void MainWin::addJSObject() {
    // Add m_launcher to JavaScript Frame as member "launcher".
    page()->mainFrame()->addToJavaScriptWindowObject(QString("launcher"), m_launcher);
}
//! [MainWindow - addJSObject]
