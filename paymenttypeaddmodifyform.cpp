/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing payment type.

*/

#include "paymenttypeaddmodifyform.h"
#include "ui_paymenttypeaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new payment type
PaymentTypeAddModifyForm::PaymentTypeAddModifyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PaymentTypeAddModifyForm)
{
    ui->setupUi(this);
    // Prevents user from resizing dialog.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lblId->hide();
    ui->lbllblId->hide();

    this->setWindowTitle("New Payment Type");

    queryType = "insertion";
}

// Constructor for updating existing payment type
PaymentTypeAddModifyForm::PaymentTypeAddModifyForm(QWidget *parent, PaymentType oldPaymentType) :
    QDialog(parent),
    ui(new Ui::PaymentTypeAddModifyForm)
{
    ui->setupUi(this);
    // Prevents user from resizing dialog.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->oldPaymentType = oldPaymentType;

    ui->lblId->setText(QString::number(oldPaymentType.getId()));
    ui->ledName->setText(oldPaymentType.getName());

    this->setWindowTitle("Update Payment Type");

    queryType = "update";
}

PaymentTypeAddModifyForm::~PaymentTypeAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Check if required fields were filled out in form.
// @return bool. True if all required fields were filled out.
bool PaymentTypeAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->ledName->text() == "")
    {
        message += " Name";
        QMessageBox::warning(this, "Error", message + "!");
        return false;
    }
    else
        return true;

}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void PaymentTypeAddModifyForm::on_pbtOk_clicked()
{
    // Check if required fields were filled out in form.
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of payment type?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newPaymentType.setId(ui->lblId->text().toInt());
    newPaymentType.setName(ui->ledName->text());

    QString queryText = "";

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    if(!conn.transaction())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    if(queryType == "insertion")
    {
        queryText = "INSERT INTO payment_type (name) "
                    "VALUES (:name); ";
    }
    else
    {

        queryText = "UPDATE payment_type ";

        if(oldPaymentType.getName() != newPaymentType.getName())
            queryText += "SET name = :name ";
        else
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "No changes made!");
            return;
        }

        queryText += " WHERE payment_type_id = :payment_type_id;";

    }

    query.prepare(queryText);

    query.bindValue(":payment_type_id", newPaymentType.getId());
    query.bindValue(":name", newPaymentType.getName());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Payment type \"" + newPaymentType.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Payment type \"" + newPaymentType.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Payment type \"" + newPaymentType.getName() + "\" " + queryType + " successful!");

    // Says push button "pbtOk" was clicked
    emit is_pbtOK_clicked(true);
    close();

}

void PaymentTypeAddModifyForm::on_pbtCancel_clicked()
{
    // Says push button "pbtOk" wasn't clicked
    emit is_pbtOK_clicked(false);
    close();
}
