/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays payment types. Allows look up, add new, delete or update existing payment type.

*/

#ifndef PAYMENTTYPESFORM_H
#define PAYMENTTYPESFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

#include "login.h"
#include "paymenttype.h"
#include "paymenttypeaddmodifyform.h"

namespace Ui {
class PaymentTypesForm;
}

class PaymentTypesForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit PaymentTypesForm(QWidget *parent = nullptr);
    ~PaymentTypesForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with information about payment types.
    // @param bool. True if table was successfully loaded.
    bool loadTable();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtDelete_clicked();
    void on_pbtAddNew_clicked();
    void on_pbtModify_clicked();

    // Executed when is_pbtOK_clicked signal from PaymentTypeAddModifyForm was emitted.
    // @param bool clicked. True if push button pbtOk was clicked.
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::PaymentTypesForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;

    // Model that contains information about payment types
    QSqlQueryModel *model;
};

#endif // PAYMENTTYPESFORM_H
