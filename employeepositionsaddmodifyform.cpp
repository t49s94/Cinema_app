/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window add new or update an existing employee position.

*/

#include "employeepositionsaddmodifyform.h"
#include "ui_employeepositionsaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new employee position.
EmployeePositionsAddModifyForm::EmployeePositionsAddModifyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmployeePositionsAddModifyForm)
{
    ui->setupUi(this);

    // Prevents user from resizing dialog
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lblId->hide();
    ui->lbllblId->hide();

    this->setWindowTitle("New Employee Position");
    queryType = "insertion";

}

// Constructor for updating an existing employee position
EmployeePositionsAddModifyForm::EmployeePositionsAddModifyForm(QWidget *parent, EmployeePosition oldPosition) :
    QDialog(parent),
    ui(new Ui::EmployeePositionsAddModifyForm)
{
    ui->setupUi(this);

    // Prevents user from resizing dialog
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->oldPosition = oldPosition;

    ui->lblId->setText(QString::number(oldPosition.getId()));
    ui->ledName->setText(oldPosition.getName());

    this->setWindowTitle("Update Employee Position");
    queryType = "update";

}

EmployeePositionsAddModifyForm::~EmployeePositionsAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Checks if required fields were filled out.
// @return bool. True if all required fields were filled out
bool EmployeePositionsAddModifyForm::areFieldsNotBlank()
{
    if(ui->ledName->text() == "")
    {
        QMessageBox::warning(this, "Error", "Please fill following fields: Name!");
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void EmployeePositionsAddModifyForm::on_pbtOk_clicked()
{
    // Checks if required fields were filled out.
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();

    // Ask if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of employee position?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    EmployeePosition newPosition;
    newPosition.setId(ui->lblId->text().toInt());
    newPosition.setName(ui->ledName->text());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with employee position " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;
    QString queryText;

    if(queryType == "insertion")
    {
        queryText = "INSERT INTO employee_position (name) "
                    "VALUES (:name);";
    }
    else
    {
        queryText = "UPDATE employee_position ";
        // Lists that contains the fields that are gonna be updated
        QStringList setsList;

        if(oldPosition.getName() != newPosition.getName())
            setsList.append(", name = :name ");

        if(setsList.length() == 0)
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "No changes made!");
            return;
        }

        // Append list that are gonna be updated to update query
        for(int i = 0; i < setsList.length(); i++)
            queryText += setsList.at(i);

        queryText += " WHERE employee_position_id = :id;";

        QString subStr(","); // String to replace.
        QString newStr("SET"); // Replacement string.
        queryText.replace(queryText.indexOf(subStr), subStr.size(), newStr); //replace first occurrence
    }

    query.prepare(queryText);

    query.bindValue(":id", newPosition.getId());
    query.bindValue(":name", newPosition.getName());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Employee position \"" + newPosition.getName() + "\" " +
                             queryType + " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Employee position \"" + newPosition.getName() + "\" " + queryType + " successful!");

    // Emit signal that means push button "pbtOK" was clicked
    emit is_pbtOK_clicked(true);
    close();

}

void EmployeePositionsAddModifyForm::on_pbtCancel_clicked()
{
    // Emit signal that means push button "pbtOK" wasn't clicked
    emit is_pbtOK_clicked(false);
    close();
}
