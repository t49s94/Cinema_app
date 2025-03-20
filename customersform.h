#ifndef CUSTOMERSFORM_H
#define CUSTOMERSFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays customers stored in database. Allows add, delete or modify existing customer.

*/

#include <QMainWindow>

#include <QDate>
#include <qsqlquerymodel.h>
#include <qmessagebox.h>

#include "login.h"
#include "customer.h"
#include "customersaddmodifyform.h"


namespace Ui {
class CustomersForm;
}

class CustomersForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit CustomersForm(QWidget *parent = nullptr);
    ~CustomersForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with customer's information.
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
    // Delete selected customers
    void on_pbtDelete_clicked();
    // Update selected row from table
    void on_pbtModify_clicked();

    void on_chkDobFrom_stateChanged(int checked);
    void on_chkDobTo_stateChanged(int checked);
    void on_chkSinceFrom_stateChanged(int checked);
    void on_chkSinceTo_stateChanged(int checked);
    void on_chkPrintDateFrom_stateChanged(int checked);
    void on_chkPrintDateTo_stateChanged(int checked);

    // Executes when is_pbtOK_clicked signal is emitted from CustomersAddModifyForm.
    // @param bool clicked. True if push button "pbtOK" was clicked
    void on_is_pbtOK_clicked(bool clicked);

    // Executes when ledPhone is foccussed in or out.
    // @param bool hasFocus. True if focused in. False if out
    void on_ledPhone_focussed(bool hasFocus);

private:
    Ui::CustomersForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about customers
    QSqlQueryModel *model;

    QStringList customerTypeList;
    // List with Ids of customer types
    QList<int> customerTypeIDList;
};

#endif // CUSTOMERSFORM_H
