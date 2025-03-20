/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays information about employee positions . Allows look up, add new, delete or update existing employee position.

*/

#ifndef EMPLOYEETYPESFORM_H
#define EMPLOYEETYPESFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

#include "login.h"
#include "employeepositionsaddmodifyform.h"
#include "employeeposition.h"

namespace Ui {
class EmployeeTypesForm;
}

class EmployeeTypesForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit EmployeeTypesForm(QWidget *parent = nullptr);
    ~EmployeeTypesForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with employee positions.
    // @return bool. True if table was successfully loaded
    bool loadTable();

private slots:

    void on_pbtLoadTable_clicked();
    // Delete selected records from table
    void on_pbtDelete_clicked();
    void on_pbtAddNew_clicked();
    // Update selected record
    void on_pbtModify_clicked();

    // Executes when push button pbtOK from EmployeePositionsAddModifyForm is clicked
    // @param bool clicked. True if clicked. False if it wasn't.
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::EmployeeTypesForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about employee positions.
    QSqlQueryModel *model;
};

#endif // EMPLOYEETYPESFORM_H
