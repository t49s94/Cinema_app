/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and funtions to connect to DB.

*/

#include "login.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Login::Login(QObject *parent) : QObject(parent)
{
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Close connection
void Login::connClose()
{
    db.close();
}

// Open connection to DB
// @param bool. True if connection was successfully established.
bool Login::connOpen()
{
    db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("");
    db.setDatabaseName("portafolio_cinema");

    return db.open();
}

// Begin transaction
// @param bool. True if transaction was successfully created.
bool Login::transaction()
{
    return db.driver()->beginTransaction();
}

// Commit transaction
// @param bool. True if transaction was successfully committed.
bool Login::commit()
{
    return db.driver()->commitTransaction();
}

// Rollback transaction
// @param bool. True if transaction was successfully rollbacked.
bool Login::rollback()
{
    return db.driver()->rollbackTransaction();
}

// Get native error code.
// @return QString. Native error code.
QString Login::nativeErrorCode()
{
    return db.lastError().nativeErrorCode();
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

QSqlDatabase Login::getDatabase()
{
    return db;
}

