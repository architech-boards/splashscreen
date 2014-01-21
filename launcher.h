#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QVariantList>
#include <QWebView>

class Launcher : public QObject
{
    Q_OBJECT
public:
    explicit Launcher(QObject *parent = 0, QWebView *web_view = 0);

signals:

public slots:

    /**
      * Creates a process and waits for its termination.
      *
      * @param program              The program to run.
      * @param change_directory     If true the program will be run in the very same directory where it resides.
      * @return                     The return value is the value returned by the terminated the process.
      */
    int                             process_execute( const QString program, bool change_directory );

    /**
      * Runs a program with the specified command line arguments and waits for its termination.
      *
      * @param program              The program to run.
      * @param arguments            The list of command line arguments to give to the process.
      * @param change_directory     If true the program will be run in the very same directory where it resides.
      * @return                     The return value is the value returned by the terminated process.
      */
    int                             process_execute( const QString program, const QVariantList &arguments, bool change_directory );

    /**
      * Runs a program and does not wait for its termination.
      *
      * @param program              The program to run.
      * @param change_directory     If true working directory of the new process will be the directory containing the program.
      * @return                     True on success, false otherwise.
      */
    bool                            process_start_detached( const QString program, bool change_directory );

    /**
      * Runs a program with the specified command line arguments and does not wait for its termination.
      *
      * @param program              The program to run.
      * @param change_directory     If true working directory of the new process will be the directory containing the program.
      * @return                     True on success, false otherwise.
      */
    bool                            process_start_detached( const QString program, const QVariantList &arguments, bool change_directory );

    /**
      * Instructs the viewer to point to the home page.
      */
    void                            go_home();

    /**
      * Exits from the splashscreen.
      */
    void                            exit_from_application();

    /**
      * Exits from the splashscreen with the specified exit value.
      */
    void                            exit_from_application( int exit_code );

    /**
      * Given a path relative to the splashscreen's one, return its corresponding absolute path.
      *
      * @param relative_path        The relative path.
      * @return                     The correspoding absolute path.
      */
    QString                         get_absolute_path( const QString relative_path );

    /**
      * Gets the list of boards made by ArchiTech as a concatenated list of each board's information.
      *
      * @return                     The boards list.
      */
    QVariant                        get_architech_boards_list();

    /**
      * Given the board alias, that is the internal name of the board used in the file system as well,
      * returns its information as a one dimension array.
      *
      * @param board_alias          The internal name of the board.
      * @return                     The information on the board.
      */
    QVariant                        get_architech_board_info( const QString board_alias );

    /**
      * Gets the list of partners as a concatenated list of each partner's information.
      *
      * @return                     The list of partners information.
      */
    QVariant                        get_partners_list();

    /**
      * Gets information about the specified partner alias. The partner alias is the internal name
      * of the partner, which is used inside the file system as well.
      *
      * @param partner_alias        The internal name of the partner.
      * @return                     The information about the partner.
      */
    QVariant                        get_partner_info( const QString partner_alias );

    /**
      * Gets the boards list of a specific partner as a concatenated list of each board's information.
      *
      * @param partner_alias        The partner alias.
      * @return                     The list of information about the partner boards.
      */
    QVariant                        get_partner_boards_list( const QString partner_alias );

    /**
      * Gets information about a specific board made by a specific partner.
      *
      * @param partner_alias        The partner alias.
      * @param board_alis           The board alias.
      * @return                     The list of attributes composing the board information.
      */
    QVariant                        get_partner_board_info( const QString partner_alias, const QString board_alias );

    /**
      * Gets the absolute path of a partner directory.
      *
      * @param partner_alias        The partner alias.
      * @return                     The absolute path of the partner directory.
      */
    QString                         get_partner_directory( const QString partner_alias );

    /**
      * Gets the absolute path of partner board directory.
      *
      * @param partner_alias        The partner alias.
      * @param board_alias          The board alias.
      * @return                     The absolute path of the partner board directory.
      */
    QString                         get_partner_board_directory( const QString partner_alias, const QString board_alias );

    /**
      * This function triggers the update of the files composing the interface.
      *
      * @return                     The exit value of the program used to run the update.
      */
    int                             update_interface();

    /**
      * Gets the absolute path to the directory containing the interface files (html etc.)
      *
      * @return                     The absolute path of the directory containing the interface files.
      */
    QString                         get_interface_directory();

private:
    /**
      * This is just a reference to the object in charge of displaying the web pages.
      */
    QWebView                        *web_view;

    /**
      * This is used to remember the splashscreen working directory.
      */
    QString                         working_directory;

    /**
      * Contains the relative path of the directory holding the .html pages used by the splashscreen.
      */
    QString                         web_site_directory;

    QStringList                     _get_architech_board_info( const QString board_directory );
    QStringList                     _get_partner_board_info( const QString partner_alias, const QString board_alias );
    QStringList                     _get_partner_info( const QString partner_directory );
    QStringList                     _get_board_info( const QString alias, const QString board_directory );
    int                             _process_execute( const QString program, const QStringList arguments, bool change_directory );
};

#endif // LAUNCHER_H
