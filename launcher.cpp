#include "launcher.h"
#include <QProcess>
#include <QApplication>
#include <QSettings>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>

#include <QWebView>
#include <QWebFrame>
#include <QFile>
#include <QDir>
#include <QTextStream>

Launcher::Launcher(QObject *parent, QWebView *web_view) :
    QObject(parent)
{
    this->web_view              = web_view;
    this->working_directory     = QDir::currentPath();
    this->web_site_directory    = "../splashscreen_interface";
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Processes

int Launcher::_process_execute( const QString program, const QStringList arguments, bool change_directory )
{
    QString
        program_directory       = QFileInfo(program).absoluteDir().absolutePath();
    int
        to_return;

    if( change_directory )
    {
        QDir::setCurrent( program_directory );
    }
    to_return                   = QProcess::execute( program, arguments );
    if( change_directory )
    {
        QDir::setCurrent( this->working_directory );
    }
    return to_return;
}

int Launcher::process_execute( const QString program, const QVariantList &arguments, bool change_directory )
{
    QStringList
        stringList;
    QListIterator<QVariant>
        i(arguments);
    QString
        program_directory       = QFileInfo(program).absoluteDir().absolutePath();
    int
        to_return;

    while( i.hasNext() )
    {
      stringList.append( i.next().toString() );
    }
    if( change_directory )
    {
        QDir::setCurrent( program_directory );
    }
    to_return                   = QProcess::execute( program, stringList );
    if( change_directory )
    {
        QDir::setCurrent( this->working_directory );
    }
    return to_return;
}

int Launcher::process_execute( const QString program, bool change_directory )
{
    QVariantList
        arguments;
    return this->process_execute( program, arguments, change_directory );
}

bool Launcher::process_start_detached( const QString program, const QVariantList &arguments, bool change_directory )
{
    QStringList
        stringList;
    QListIterator<QVariant>
        i(arguments);
    QString
        program_directory       = QFileInfo(program).absoluteDir().absolutePath();
    bool
        to_return;

    while( i.hasNext() )
    {
      stringList.append( i.next().toString() );
    }
    if( change_directory )
    {
        QDir::setCurrent( program_directory );
    }
    to_return                   = QProcess::startDetached( program, stringList );
    if( change_directory )
    {
        QDir::setCurrent( this->working_directory );
    }
    return to_return;
}

bool Launcher::process_start_detached( const QString program, bool change_directory )
{
    QVariantList
        arguments;
    return this->process_start_detached( program, arguments, change_directory );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Exit

void Launcher::exit_from_application( int exit_code )
{
    exit( exit_code );
}

void Launcher::exit_from_application()
{
    this->exit_from_application( 0 );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Navigation

void Launcher::go_home()
{
    QUrl startURL           = "file://" + this->get_absolute_path( this->web_site_directory + "/index.html" );
    this->web_view->setUrl(startURL);    
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Helpers

QString read_txt( QString filepath )
{
    QFile
        f( filepath );
    QString
        txt;
    if( !f.open(QFile::ReadOnly | QFile::Text) )
    {
        return "";
    }
    QTextStream in(&f);
    txt                     = in.readAll();
    if( txt.at( txt.length() - 1 ) == '\n' )
    {
        txt.chop( 1 );
    }
    return txt;
}


QString Launcher::get_absolute_path( const QString relative_path )
{
    return QDir( QDir( this->working_directory ).filePath( relative_path ) ).canonicalPath();
}

QStringList Launcher::_get_board_info( const QString alias, const QString board_directory )
{
    QStringList
        board_data;
    QString
        data;
    QString
        board_splascreen_directory  = board_directory + "/splashscreen";

    // Saving board alias
    data                = alias;
    board_data << data;

    // Reading board name
    data                = read_txt( board_splascreen_directory + "/name.txt" );
    if( data == "" )
    {
        goto exit;
    }
    board_data << data;

    // Reading board image filepath
    data                = read_txt( board_splascreen_directory + "/board_image.txt" );
    if( data == "" )
    {
        goto exit;
    }
    data                = board_splascreen_directory + "/" + data;
    board_data << data;

    // Reading short description
    data                = read_txt( board_splascreen_directory + "/short_description.txt" );
    if( data == "" )
    {
        goto exit;
    }
    board_data << data;


    // Reading long description
    data                = read_txt( board_splascreen_directory + "/long_description.txt" );
    if( data == "" )
    {
        goto exit;
    }
    board_data << data;

    board_data << board_splascreen_directory + "/run_hob";
    board_data << board_splascreen_directory + "/run_bitbake";
    board_data << board_splascreen_directory + "/run_cross-compiler";
    board_data << board_splascreen_directory + "/run_eclipse";
    board_data << board_splascreen_directory + "/run_qt-creator";
    board_data << board_splascreen_directory + "/run_images-folder";
    board_data << board_splascreen_directory + "/run_documentation";
    board_data << board_splascreen_directory + "/run_update";

exit:
    return board_data;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Architech boards

QStringList Launcher::_get_architech_board_info( const QString board_directory )
{
    return _get_board_info( board_directory, this->get_absolute_path( "../architech/" + board_directory ) );
}

QVariant Launcher::get_architech_board_info( const QString board_directory )
{
    return QVariant::fromValue( _get_architech_board_info( board_directory) );
}

QVariant Launcher::get_architech_boards_list()
{
    QStringList
        boards_list;
    QDir
        architech_directory( this->get_absolute_path( "../architech" ) );
    QStringList
        name_filters;
    name_filters << "*";
    QStringList
        directories         = architech_directory.entryList( name_filters, QDir::Filters( QDir::Dirs ) );
    for( int i = 0; i < directories.size(); i++ )
    {
        QString
            board_directory = directories.at( i );
        if( board_directory == "." || board_directory == ".." )
        {
            continue;
        }

        // Adding board information
        boards_list << this->_get_architech_board_info( board_directory );

    }
    return QVariant::fromValue( boards_list );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Partner boards

QStringList Launcher::_get_partner_board_info( const QString partner_alias, const QString board_alias )
{
    return _get_board_info( board_alias, this->get_absolute_path( "../partners/" + partner_alias + "/" + board_alias ) );
}

QVariant Launcher::get_partner_board_info( const QString partner_alias, const QString board_alias )
{
    return QVariant::fromValue( _get_partner_board_info( partner_alias, board_alias ) );
}

QVariant Launcher::get_partner_boards_list( const QString partner_alias )
{
    QStringList
        boards_list;
    QDir
        partner_directory( this->get_absolute_path( "../partners/" + partner_alias ) );
    QStringList
        name_filters;
    name_filters << "*";
    QStringList
        directories         = partner_directory.entryList( name_filters, QDir::Filters( QDir::Dirs ) );
    for( int i = 0; i < directories.size(); i++ )
    {
        QString
            board_directory = directories.at( i );
        if( board_directory == "." || board_directory == ".." || board_directory == "splashscreen" )
        {
            continue;
        }

        // Adding board information
        boards_list << this->_get_partner_board_info( partner_alias, board_directory );

    }
    return QVariant::fromValue( boards_list );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Partners

QStringList Launcher::_get_partner_info( const QString partner_directory )
{
    QStringList
        board_data;
    QString
        data,
        partner_splashscreen_directory  = this->get_absolute_path( "../partners/" + partner_directory + "/splashscreen" );
    QFile
        index_html( partner_splashscreen_directory + "/index.html" );

    // Saving partner alias
    data                = partner_directory;
    board_data << data;

    // Reading partner name
    data                = read_txt( partner_splashscreen_directory + "/name.txt" );
    if( data == "" )
    {
        goto exit;
    }
    board_data << data;

    // Reading logo filepath
    data                = read_txt( partner_splashscreen_directory + "/logo.txt" );
    if( data == "" )
    {
        goto exit;
    }
    data                = partner_splashscreen_directory + "/" + data;
    board_data << data;

    // Reading short description
    data                = read_txt( partner_splashscreen_directory + "/short_description.txt" );
    if( data == "" )
    {
        goto exit;
    }
    board_data << data;


    // Reading long description
    data                = read_txt( partner_splashscreen_directory + "/long_description.txt" );
    if( data == "" )
    {
        goto exit;
    }
    board_data << data;

    // Checking if index.html exists
    if(!index_html.exists())
    {
        board_data << "";
    }
    else
    {
        board_data << partner_splashscreen_directory + "/index.html";
    }

exit:
    return board_data;
}

QVariant Launcher::get_partner_info( const QString partner_directory )
{
    return QVariant::fromValue( _get_partner_info( partner_directory) );
}

QVariant Launcher::get_partners_list()
{
    QStringList
        partners_list;
    QDir
        partners_directory( this->get_absolute_path( "../partners" ) );
    QStringList
        name_filters;
    name_filters << "*";
    QStringList
        directories             = partners_directory.entryList( name_filters, QDir::Filters( QDir::Dirs ) );
    for( int i = 0; i < directories.size(); i++ )
    {
        QString
            partner_directory   = directories.at( i );
        if( partner_directory == "." || partner_directory == ".." )
        {
            continue;
        }

        // Adding board information
        partners_list << this->_get_partner_info( partner_directory );

    }
    return QVariant::fromValue( partners_list );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Partners paths

QString Launcher::get_partner_directory( const QString partner_alias )
{
    return this->get_absolute_path( "../partners/" + partner_alias );
}

QString Launcher::get_partner_board_directory( const QString partner_alias, const QString board_alias )
{
    return this->get_absolute_path( "../partners/" + partner_alias + "/" + board_alias );
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Interface utils

int Launcher::update_interface()
{
    QString
        interface_directory     =  this->get_interface_directory();
    QStringList
        arguments;
    arguments                   << interface_directory;
    return this->_process_execute( this->working_directory + "/scripts/update_interface", arguments, true );
}

QString Launcher::get_interface_directory()
{
    return this->get_absolute_path( this->web_site_directory );
}
