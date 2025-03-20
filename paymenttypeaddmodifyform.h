/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing payment type.

*/

#ifndef PAYMENTTYPEADDMODIFYFORM_H
#define PAYMENTTYPEADDMODIFYFORM_H

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "paymenttype.h"

namespace Ui {
class PaymentTypeAddModifyForm;
}

class PaymentTypeAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new payment type
    explicit PaymentTypeAddModifyForm(QWidget *parent = nullptr);
    // Constructor for updating existing payment type
    explicit PaymentTypeAddModifyForm(QWidget *parent, PaymentType oldPaymentType);
    ~PaymentTypeAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Check if required fields were filled out in form.
    // @return bool. True if all required fields were filled out.
    bool areFieldsNotBlank();

signals:

    void is_pbtOK_clicked(bool clicked);

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::PaymentTypeAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // Original data of payment type
    PaymentType oldPaymentType;
    // Updated data of payment type
    PaymentType newPaymentType;
};

#endif // PAYMENTTYPEADDMODIFYFORM_H
