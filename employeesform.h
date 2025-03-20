/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays employees stored in database. Allows add, delete or modify existing employee.

*/

#ifndef EMPLOYEESFORM_H
#define EMPLOYEESFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QDate>

#include "login.h"
#include "employeeaddmodifyform.h"
#include "mylineedit.h"

namespace Ui {
class EmployeesForm;
}

class EmployeesForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit EmployeesForm(QWidget *parent = nullptr);
    ~EmployeesForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with employee's information.
    // @return bool. True if table was sucessfully loaded.
    bool loadTable();

    // Resets fields to default value.
    void resetFields();
    // Checks if fields have correct data.
    // @return bool. True if they do.
    bool validateFields();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    void on_pbtDelete_clicked();
    void on_pbtModify_clicked();

    void on_chkDobFrom_stateChanged(int checked);
    void on_chkDobTo_stateChanged(int checked);
    void on_chkHiringDateFrom_stateChanged(int checked);
    void on_chkHiringDateTo_stateChanged(int checked);

    // Executes when is_pbtOK_clicked signal is emitted from CustomersAddModifyForm.
    // @param bool clicked. True if push button "pbtOK" was clicked
    void on_is_pbtOK_clicked(bool clicked);

    // Executes when ledPhone is foccussed in or out.
    // @param bool hasFocus. True if focused in. False if out
    void on_ledPhone_focussed(bool hasFocus);
    // Executes when ledSSN is foccussed in or out.
    // @param bool hasFocus. True if focused in. False if out
    void on_ledSSN_focussed(bool hasFocus);

private:
    Ui::EmployeesForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about customers
    QSqlQueryModel *model;

    // List of employee positions
    QStringList employeePositionsList;
    // List of Ids of employee positions
    QList<int> employeePositionsIDList;
};

#endif // EMPLOYEESFORM_H
