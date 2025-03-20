#ifndef BOXEMPLOYEEFORM_H
#define BOXEMPLOYEEFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This Window is the main menu for the Box office employees. Allows them to create new transactions for customers who wants
to watch movie(s).

*/

#include <QMainWindow>

#include "employee.h"
#include "boxofficetransactionaddform.h"
#include <QMessageBox>

namespace Ui {
class BoxEmployeeForm;
}

class BoxEmployeeForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    explicit BoxEmployeeForm(QWidget *parent, Employee employee);
    ~BoxEmployeeForm();

private slots:
    // Create an instance of BoxOfficeTransactionAddForm for a new transaction.
    void on_pbtNewTransaction_clicked();

    // Logs out employee.
    void on_pbtLogOut_clicked();

private:
    Ui::BoxEmployeeForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    Employee employee;
};

#endif // BOXEMPLOYEEFORM_H
