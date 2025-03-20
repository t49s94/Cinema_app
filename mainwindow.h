/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows sing in to the application.

*/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtSql>
#include <QSqlDatabase>
#include <QMessageBox>

#include "boxemployeeform.h"
#include "generalmanagerform.h"
#include "login.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Loads a window and hides this one.
    // @param QWidget *window. Window that is gonna be shown
    void loadWindow(QWidget *window);

private slots:

    void on_pbtLogin_clicked();

private:
    Ui::MainWindow *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    QString username;
    QString password;
};
#endif // MAINWINDOW_H
