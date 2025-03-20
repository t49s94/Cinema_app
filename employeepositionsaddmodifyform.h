/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window add new or update an existing employee position.

*/

#ifndef EMPLOYEEPOSITIONSADDMODIFYFORM_H
#define EMPLOYEEPOSITIONSADDMODIFYFORM_H

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "employeeposition.h"

namespace Ui {
class EmployeePositionsAddModifyForm;
}

class EmployeePositionsAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new employee position.
    explicit EmployeePositionsAddModifyForm(QWidget *parent = nullptr);
    // Constructor for updating an existing employee position
    explicit EmployeePositionsAddModifyForm(QWidget *parent, EmployeePosition oldPosition);

    ~EmployeePositionsAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Checks if required fields were filled out.
    // @return bool. True if all required fields were filled out
    bool areFieldsNotBlank();

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::EmployeePositionsAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update.
    QString queryType;

    // Original data of employee position
    EmployeePosition oldPosition;

signals:

    // Says if push button "pbtOk" was clicked.
    // @param bool clicked. True if it was clicked.
    void is_pbtOK_clicked(bool clicked);
};

#endif // EMPLOYEEPOSITIONSADDMODIFYFORM_H
