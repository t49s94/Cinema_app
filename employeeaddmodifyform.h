/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window adds new or update exisisting employee.

*/

#ifndef EMPLOYEEADDMODIFYFORM_H
#define EMPLOYEEADDMODIFYFORM_H

#include <QDialog>

#include <qmessagebox.h>
#include <QListWidgetItem>

#include "login.h"
#include "employee.h"
#include "employeeposition.h"


namespace Ui {
class EmployeeAddModifyForm;
}

class EmployeeAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new employee.
    explicit EmployeeAddModifyForm(QWidget *parent, QList<int> employeePositionIDList, QStringList employeePositionList);
    // Constructor for updating existing employee.
    explicit EmployeeAddModifyForm(QWidget *parent, QList<int> employeePositionIDList,
                                    QStringList employeePositionList, Employee oldEmployee);
    ~EmployeeAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initializes some features of form.
    // @param QList<int> employeePositionIDList. List with Ids of employee positions.
    // @param QStringList employeePositionList. List with employee positions.
    void initializeForm(QList<int> employeePositionIDList, QStringList employeePositionList);

    // Checks if data input is correct.
    // @return bool. True if data is correct.
    bool validateFields();
    // Checks if all required fields were filled out.
    // @return bool. True if data if they where.
    bool areFieldsNotBlank();

    // Sets the text of update query.
    // @param QString* queryText. Holds text of update query.
    // @return bool. True if text was set successfully.
    bool setUpdateQueryText(QString* queryText);

signals:

    void is_pbtOK_clicked(bool clicked);

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

    void on_chkPassword_stateChanged(int checked);

    // Executes when ledPhone is foccussed in or out.
    // @param bool hasFocus. True if it was focussed in. False if focussed out.
    void on_ledPhone_focussed(bool hasFocus);
    // Executes when ledSSN is foccussed in or out.
    // @param bool hasFocus. True if it was focussed in. False if focussed out.
    void on_ledSSN_focussed(bool hasFocus);

private:
    Ui::EmployeeAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // QList with Ids of employee positions
    QList<int> employeePositionIDList;
    // QList with employee positions
    QStringList employeePositionList;

    // Original employee data
    Employee oldEmployee;
    // Updated employee data
    Employee newEmployee;
};

#endif // EMPLOYEEADDMODIFYFORM_H
