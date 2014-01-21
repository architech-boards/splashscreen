#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWebView>
#include "launcher.h"

//QT_BEGIN_NAMESPACE
class QNetworkDiskCache;
//QT_END_NAMESPACE

class MainWin : public QWebView
{
    Q_OBJECT

public:
    explicit MainWin(QWidget * parent = 0);

private:
    Launcher * m_launcher;

    QNetworkAccessManager * m_network;
    QNetworkDiskCache * m_cache;

private slots:
    void addJSObject();

};
#endif
