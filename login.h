/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and funtions to connect to DB.

*/

#ifndef LOGIN_H
#define LOGIN_H

#include <QMainWindow>

#include <QObject>
#include <QtSql>

class Login : public QObject
{
    Q_OBJECT
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit Login(QObject *parent = nullptr);

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Close connection
    void connClose();
    // Open connection to DB
    // @param bool. True if connection was successfully established.
    bool connOpen();

    // Begin transaction
    // @param bool. True if transaction was successfully created.
    bool transaction();
    // Commit transaction
    // @param bool. True if transaction was successfully committed.
    bool commit();
    // Rollback transaction
    // @param bool. True if transaction was successfully rollbacked.
    bool rollback();

    // Get native error code.
    // @return QString. Native error code.
    QString nativeErrorCode();

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    QSqlDatabase getDatabase();

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    QSqlDatabase db;


signals:

};

#endif // LOGIN_H
