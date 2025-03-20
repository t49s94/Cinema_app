#include "customertypesaddmodifyform.h"
#include "ui_customertypesaddmodifyform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows add new or update existing customer type.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new customer type
CustomerTypesAddModifyForm::CustomerTypesAddModifyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomerTypesAddModifyForm)
{
    ui->setupUi(this);
    // Prevents user from resizing window
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lblId->hide();
    ui->lbllblId->hide();

    this->setWindowTitle("New Customer Type");

    queryType = "insertion";

}

// Constructor for updating existing customer type
CustomerTypesAddModifyForm::CustomerTypesAddModifyForm(QWidget *parent, CustomerType oldCustomerType) :
    QDialog(parent),
    ui(new Ui::CustomerTypesAddModifyForm)
{
    ui->setupUi(this);
    // Prevents user from resizing window
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->oldCustomerType = oldCustomerType;

    ui->lblId->setText(QString::number(oldCustomerType.getId()));
    ui->ledName->setText(oldCustomerType.getName());
    ui->spbCumulation->setValue(oldCustomerType.getCumulation());
    ui->spbVisitsRequired->setValue(oldCustomerType.getVisitsRequired());

    this->setWindowTitle("Update Customer Type");

    queryType = "update";

}

CustomerTypesAddModifyForm::~CustomerTypesAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Checks if required fields were filled out.
// @return bool. True if all required fields were filled out
bool CustomerTypesAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->ledName->text() == "")
        message += ", Name";
    if(ui->spbCumulation->value() != 0)
        message += ", Cumulation";
    if(ui->spbVisitsRequired != 0)
        message += ", Visits required";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    // Returns true if all the required fields were filled up
    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void CustomerTypesAddModifyForm::on_pbtOk_clicked()
{
    // Check if required fields are not blank
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    // Asks if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of cast position?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    CustomerType newCustomerType;
    newCustomerType.setId(ui->lblId->text().toInt());
    newCustomerType.setName(ui->ledName->text());
    newCustomerType.setCumulation(ui->spbCumulation->value());
    newCustomerType.setVisitsRequired(ui->spbVisitsRequired->value());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with customer type " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;
    QString queryText;

    if(queryType == "insertion")
    {
        queryText = "INSERT INTO customer_type (name, cumulation, visits_required) "
                    "VALUES (:name, :cumulation, :visitsRequired);";
        query.bindValue(":name", newCustomerType.getName());
        query.bindValue(":cumulation", newCustomerType.getCumulation());
        query.bindValue(":visitsRequired", newCustomerType.getVisitsRequired());
    }
    else
    {
        queryText = "UPDATE customer_type ";
        // List of fields that are gonna be updated
        QStringList setsList;

        if(oldCustomerType.getName() != newCustomerType.getName())
            setsList.append(", name = :name ");
        if(oldCustomerType.getCumulation() != newCustomerType.getCumulation())
            setsList.append(", cumulation = :cumulation ");
        if(oldCustomerType.getVisitsRequired() != newCustomerType.getVisitsRequired())
            setsList.append(", visits_required = :visitsRequired ");

        if(setsList.length() == 0)
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "No changes made!");
            return;
        }

        // appends list of fields to update query text
        for(int i = 0; i < setsList.length(); i++)
            queryText += setsList.at(i);

        queryText += " WHERE customer_type_id = :id;";

        QString subStr(","); // String to replace.
        QString newStr("SET"); // Replacement string.
        queryText.replace(queryText.indexOf(subStr), subStr.size(), newStr); //replace first occurrence


    }

    query.prepare(queryText);

    query.bindValue(":id", newCustomerType.getId());
    query.bindValue(":name", newCustomerType.getName());
    query.bindValue(":cumulation", newCustomerType.getCumulation());
    query.bindValue(":visitsRequired", newCustomerType.getVisitsRequired());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Customer type \"" + newCustomerType.getName() + "\" " +
                             queryType + " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Customer type \"" + newCustomerType.getName() + "\" " + queryType + " successful!");
    emit is_pbtOK_clicked(true);
    close();

}

void CustomerTypesAddModifyForm::on_pbtCancel_clicked()
{
    emit is_pbtOK_clicked(false);
    close();
}
