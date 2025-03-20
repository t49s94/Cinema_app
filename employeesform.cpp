/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays a table with employee information. Allows to look up, add new, update or delete employee.

*/

#include "employeesform.h"
#include "ui_employeesform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

EmployeesForm::EmployeesForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EmployeesForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledFirstName->setPlaceholderText("Any");
    ui->ledMiddleName->setPlaceholderText("Any");
    ui->ledLastName->setPlaceholderText("Any");
    ui->ledUsername->setPlaceholderText("Any");
    ui->ledSSN->setPlaceholderText("999-999-9999");
    ui->ledEmail->setPlaceholderText("Any");
    ui->ledPhone->setPlaceholderText("01 (999) 999-9999");
    ui->ledStreetAddress->setPlaceholderText("Any");
    ui->ledCity->setPlaceholderText("Any");
    ui->ledZipcode->setPlaceholderText("Any");

    QStringList genderList = { "Any", "Male", "Female" };
    ui->cbxGender->addItems(genderList);

    QStringList stateCodeList = {
        "Any", "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "DC", "FL", "GA", "HI", "ID",
        "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE",
        "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "PR", "RI", "SC", "SC",
        "SD", "TN", "TX", "UT", "VT", "VA", "VI", "WA", "WV", "WI", "WY"
    };
    ui->cbxStateCode->addItems(stateCodeList);

    ui->dedDobTo->setDate(QDate::currentDate());
    ui->dedHiringDateTo->setDate(QDate::currentDate());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employee positions!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM employee_position;");

    employeePositionsList.append("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employee positions!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        employeePositionsIDList.append(query.value(0).toInt());
        employeePositionsList.append(query.value(1).toString());
    }

    ui->cbxEmployeePosition->addItems(employeePositionsList);

    conn.connClose();

    // Makes ledZipcode allows only 5 digits
    QRegExp rx("\\d{5}");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->ledZipcode->setValidator(validator);

    // Executes slot when ledPhone is focussed in or out.
    connect (ui->ledPhone, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledPhone_focussed(bool)));
    // Executes slot when ledSSN is focussed in or out.
    connect (ui->ledSSN, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledSSN_focussed(bool)));

}

EmployeesForm::~EmployeesForm()
{
    if (model != 0)
        delete model;
    delete ui->tvwTable->model();
    delete ui->ledZipcode->validator();
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with employee's information.
// @return bool. True if table was successfully loaded.
bool EmployeesForm::loadTable()
{
    // Checks if fields have correct data.
    if(!validateFields())
        return false;

    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employees!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT e.employee_id AS ID, e.first_name AS 'First name', e.middle_name AS 'Middle name', e.last_name AS 'Middle name', "
                "e.username AS Username, e.ssn AS SSN, p.name AS 'Employee position', e.date_of_birth AS 'Date of birth', e.gender AS Gender, "
                "e.email AS Email, e.phone AS Phone, e.street_address AS 'Street address', e.city AS City, "
                "e.state_code AS 'State code', e.zipcode AS Zipcode, e.salary AS Salary, e.hiring_date AS 'Hiring date', "
                "p.employee_position_id "
                "FROM employee e "
                "INNER JOIN employee_position p "
                "ON e.employee_position_id = p.employee_position_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Get values from fields and append them to query Text

    QString firstName = ui->ledFirstName->text();
    queryText += firstName != ""? " AND e.first_name = :first_name" : "";

    QString middleName = ui->ledMiddleName->text();
    queryText += middleName != ""? " AND e.middle_name = :middle_name" : "";

    QString lastName = ui->ledLastName->text();
    queryText += lastName != ""? " AND e.last_name = :last_name" : "";

    QString username = ui->ledUsername->text();
    queryText += username != ""? " AND e.username = :username" : "";

    QString ssn = ui->ledSSN->text();
    ssn.replace(QString("-"), QString(""));
    queryText += ssn != ""? " AND e.ssn = :ssn" : "";

    QString employeePosition = ui->cbxEmployeePosition->currentText();
    queryText += employeePosition != "Any"? " AND p.name = :name" : "";

    QString dobFrom = ui->dedDobFrom->date().toString("yyyy-M-d");
    queryText += ui->chkDobFrom->isChecked()? " AND e.date_of_birth >= :dobFrom" : "";

    QString dobTo = ui->dedDobTo->date().toString("yyyy-M-d");
    queryText += ui->chkDobTo->isChecked()? " AND e.date_of_birth <= :dobTo" : "";

    QString email = ui->ledEmail->text();
    queryText += email != ""? " AND e.email = :email" : "";

    QString phone = ui->ledPhone->text();
    phone.replace(QString("-"), QString(""));
    phone.replace(QString("("), QString(""));
    phone.replace(QString(")"), QString(""));
    queryText += phone != ""? " AND e.phone = :phone" : "";

    QString streetAddress = ui->ledStreetAddress->text();
    queryText += streetAddress != ""? " AND e.street_address = :street_address" : "";

    QString city = ui->ledCity->text();
    queryText += city != ""? " AND e.city = :city" : "";

    QString stateCode = ui->cbxStateCode->currentText();
    queryText += stateCode != "Any"? " AND e.state_code = :state_code" : "";

    QString zipcode = ui->ledZipcode->text();
    queryText += zipcode != ""? " AND e.zipcode = :zipcode" : "";

    QString salaryFrom = QString::number(ui->dsbSalaryFrom->value(), 'f', 2);
    queryText += salaryFrom != "0.00" ? " AND e.salary >= :salaryFrom" : "";

    QString salaryTo = QString::number(ui->dsbSalaryTo->value(), 'f', 2);
    queryText += salaryTo != "0.00"? " AND e.salary <= :salaryTo" : "";

    QString hiringDateFrom = ui->dedHiringDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkHiringDateFrom->isChecked()? " AND e.hiring_date >= :hiringDateFrom" : "";

    QString hiringDateTo = ui->dedHiringDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkHiringDateTo->isChecked()? " AND e.hiring_date <= :hiringDateTo" : "";

    QString gender = "";
    switch (ui->cbxGender->currentIndex()) {
        case 1:
            gender = "M";
            queryText += " AND e.gender = :gender";
            break;
        case 2:
            gender = "F";
            queryText +=" AND e.gender = :gender";
    }

    query.prepare( queryText + " ORDER BY e.employee_id;");

    query.bindValue(":first_name", firstName);
    query.bindValue(":middle_name", middleName);
    query.bindValue(":last_name", lastName);
    query.bindValue(":username", username);
    query.bindValue(":ssn", ssn);
    query.bindValue(":name", employeePosition);
    query.bindValue(":dobFrom", dobFrom);
    query.bindValue(":dobTo", dobTo);
    query.bindValue(":gender", gender);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":street_address", streetAddress);
    query.bindValue(":city", city);
    query.bindValue(":state_code", stateCode);
    query.bindValue(":zipcode", zipcode);
    query.bindValue(":salaryFrom", salaryFrom);
    query.bindValue(":salaryTo", salaryTo);


    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employees!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

    // Model that sorts data in table
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);
    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    ui->tvwTable->hideColumn(17);

    conn.connClose();
    return true;

}

// Resets fields to default value.
void EmployeesForm::resetFields()
{
    ui->ledFirstName->setText("");
    ui->ledMiddleName->setText("");
    ui->ledLastName->setText("");
    ui->ledUsername->setText("");
    ui->ledSSN->setText("");
    ui->cbxEmployeePosition->setCurrentIndex(0);
    ui->chkDobFrom->setCheckState(Qt::Unchecked);
    ui->chkDobTo->setCheckState(Qt::Unchecked);
    ui->dedDobTo->setDate(QDate::currentDate());
    ui->cbxGender->setCurrentIndex(0);
    ui->ledEmail->setText("");
    ui->ledPhone->setText("");
    ui->ledPhone->setInputMask("");
    ui->ledStreetAddress->setText("");
    ui->ledCity->setText("");
    ui->cbxStateCode->setCurrentIndex(0);
    ui->ledZipcode->setText("");
    ui->dsbSalaryFrom->setValue(0);
    ui->dsbSalaryTo->setValue(0);
    ui->chkHiringDateFrom->setCheckState(Qt::Unchecked);
    ui->chkHiringDateTo->setCheckState(Qt::Unchecked);
    ui->dedHiringDateTo->setDate(QDate::currentDate());
}

// Checks if fields have correct data.
// @return bool. True if they do.
bool EmployeesForm::validateFields()
{
    // Error message
    QString message = "Invalid input:";

    // SSN format
    QRegExp rx("(\\d{3}-\\d{3}-\\d{4}|\\d{0}|--)");
    QRegExpValidator v(rx, 0);
    // Position where validation check will start from
    int pos = 0;

    // Email address format
    QRegExp mailRX("(\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b|d{0})");
    mailRX.setCaseSensitivity(Qt::CaseInsensitive);
    mailRX.setPatternSyntax(QRegExp::RegExp);

    QString ssn = ui->ledSSN->text();
    QString email = ui->ledEmail->text();
    QString phone = ui->ledPhone->text();
    QString zipcode = ui->ledZipcode->text();

    if(v.validate(ssn, pos) != 2)
        message += ", SSN";
    if(!mailRX.exactMatch(email))
        message += ", email";

    // Phone number format
    rx.setPattern("(\\d{0,2}\\s\\(\\d{3}\\)\\s\\d{3}-\\d{4}|d{0}|\\(\\)-)");
    v.setRegExp(rx);

    if(v.validate(phone, pos) != 2)
        message += ", phone";

    // Zipcode format
    rx.setPattern("(\\d{5}|\\d{0})");
    v.setRegExp(rx);

    if(v.validate(zipcode, pos) != 2)
        message += ", zipcode";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Invalid input:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void EmployeesForm::on_pbtLoadTable_clicked()
{
    if(loadTable())
    {
        ui->pbtAddNew->setEnabled(true);
        ui->pbtDelete->setEnabled(true);
        ui->pbtModify->setEnabled(true);

    }
    else
    {
        ui->pbtAddNew->setEnabled(false);
        ui->pbtDelete->setEnabled(false);
        ui->pbtModify->setEnabled(false);
    }
}

void EmployeesForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void EmployeesForm::on_pbtAddNew_clicked()
{
    EmployeeAddModifyForm *addForm = new EmployeeAddModifyForm(this, employeePositionsIDList, employeePositionsList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

// Deletes selected rows from table
void EmployeesForm::on_pbtDelete_clicked()
{
    // Ask if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of employee(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Holds Ids of employees that are gonna be deleted
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate QVector<int> ids.
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there's an error while deleting employees
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete employee(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete employee(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete employees
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM employee "
                     "WHERE employee_id = :employee_id;");
       query.bindValue(":employee_id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete employee(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", "Employee(s) deleted!");
       loadTable();
   }
}

// Update selected row from table
void EmployeesForm::on_pbtModify_clicked()
{
    // Make sure user only selected a single row
    if(ui->tvwTable->selectionModel()->selectedRows().count() == 0)
    {
        QMessageBox::warning(this, "Error", "No row was selected!");
        return;
    }
    else if(ui->tvwTable->selectionModel()->selectedRows().count() > 1)
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    Employee e;
    e.setId(index.sibling(index.row(),0).data().toInt());
    e.setFirstName(index.sibling(index.row(),1).data().toString());
    e.setMiddleName(index.sibling(index.row(),2).data().toString());
    e.setLastName(index.sibling(index.row(),3).data().toString());
    e.setUsername(index.sibling(index.row(),4).data().toString());
    e.setSsn(index.sibling(index.row(),5).data().toString());
    e.setPosition(EmployeePosition(index.sibling(index.row(),17).data().toInt(),
                                   index.sibling(index.row(),6).data().toString()));
    e.setDob(index.sibling(index.row(),7).data().toString());
    e.setGender(index.sibling(index.row(),8).data().toChar());
    e.setEmail(index.sibling(index.row(),9).data().toString());
    e.setPhone(index.sibling(index.row(),10).data().toString());
    e.setStreetAddress(index.sibling(index.row(),11).data().toString());
    e.setCity(index.sibling(index.row(),12).data().toString());
    e.setStateCode(index.sibling(index.row(),13).data().toString());
    e.setZipcode(index.sibling(index.row(),14).data().toString());
    e.setSalary(index.sibling(index.row(),15).data().toDouble());
    e.setHiringDate(index.sibling(index.row(),16).data().toString());

    EmployeeAddModifyForm *modifyForm = new EmployeeAddModifyForm(this, employeePositionsIDList, employeePositionsList, e);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
    delete modifyForm;
}

// Executes when is_pbtOK_clicked signal is emitted from EmployeeAddModifyForm.
// @param bool clicked. True if push button "pbtOK" was clicked
void EmployeesForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

void EmployeesForm::on_chkDobFrom_stateChanged(int checked)
{
    ui->dedDobFrom->setEnabled(checked);
}

void EmployeesForm::on_chkDobTo_stateChanged(int checked)
{
    ui->dedDobTo->setEnabled(checked);
}

void EmployeesForm::on_chkHiringDateFrom_stateChanged(int checked)
{
    ui->dedHiringDateFrom->setEnabled(checked);
}

void EmployeesForm::on_chkHiringDateTo_stateChanged(int checked)
{
    ui->dedHiringDateTo->setEnabled(checked);
}

// Executes when ledPhone is foccussed in or out.
void EmployeesForm::on_ledPhone_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledPhone->setInputMask("dd (999) 999-9999");
    else
        // If after focussing out the user leave it blank, delete input mask
        if( ui->ledPhone->text() == " () -")
            ui->ledPhone->setInputMask("");
}

// Executes when ledSSN is foccussed in or out.
void EmployeesForm::on_ledSSN_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledSSN->setInputMask("999-999-9999");
    else
        // If after focussing out the user leave it blank, delete input mask
        if( ui->ledSSN->text() == "--")
            ui->ledSSN->setInputMask("");
}
