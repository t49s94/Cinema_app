/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays theater types stored in DB. Allows look up, add new, delete or update existing theater type.

*/

#ifndef THEATERTYPESFORM_H
#define THEATERTYPESFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

#include "login.h"
#include "theatertype.h"
#include "theatertypeaddmodifyform.h"

namespace Ui {
class TheaterTypesForm;
}

class TheaterTypesForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit TheaterTypesForm(QWidget *parent = nullptr);
    ~TheaterTypesForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with information about theater types.
    // @param bool. True if table was successfully loaded.
    bool loadTable();

private slots:

    void on_pbtLoadTable_clicked();
    // Delete selected theater types from table
    void on_pbtDelete_clicked();
    void on_pbtAddNew_clicked();
    // Update selected theater types from table
    void on_pbtModify_clicked();

    // Executes when is_pbtOK_clicked signal from TheaterTypeAddModifyForm is emitted.
    // @param bool clicked. True if push button "pbtOK" was clicked.
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::TheaterTypesForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Contains information about theater types.
    QSqlQueryModel *model;
};

#endif // THEATERTYPESFORM_H
