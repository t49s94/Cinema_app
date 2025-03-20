#ifndef CUSTOMERTYPESADDMODIFYFORM_H
#define CUSTOMERTYPESADDMODIFYFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows add new or update existing customer type.

*/

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "customertype.h"

namespace Ui {
class CustomerTypesAddModifyForm;
}

class CustomerTypesAddModifyForm : public QDialog
{
    Q_OBJECT

public:
    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new customer type
    explicit CustomerTypesAddModifyForm(QWidget *parent = nullptr);
    // Constructor for updating existing customer type
    explicit CustomerTypesAddModifyForm(QWidget *parent, CustomerType oldCustomerType);
    ~CustomerTypesAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Checks if required fields were filled out.
    // @return bool. True if all required fields were filled out
    bool areFieldsNotBlank();

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::CustomerTypesAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert of Update
    QString queryType;

    // Original data from customer type
    CustomerType oldCustomerType;

signals:

    // Is emitted when push button "pbtOk" is clicked
    // @param bool clicked. False if "pbtOk" wasn't clicked
    void is_pbtOK_clicked(bool clicked);
};

#endif // CUSTOMERTYPESADDMODIFYFORM_H
