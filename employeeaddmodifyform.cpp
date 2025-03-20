/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window adds new or update exisisting employee.

*/

#include "employeeaddmodifyform.h"
#include "ui_employeeaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new employee.
EmployeeAddModifyForm::EmployeeAddModifyForm(QWidget *parent, QList<int> employeePositionIDList, QStringList employeePositionList) :
    QDialog(parent),
    ui(new Ui::EmployeeAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(employeePositionIDList, employeePositionList);

    ui->lblId->hide();
    ui->lbllblId->hide();
    // A password must be set for new employee
    ui->chkPassword->hide();
    ui->chkPassword->setCheckState(Qt::Checked);

    this->setWindowTitle("New Employee");
    queryType = "insertion";
}

// Constructor for updating existing employee.
EmployeeAddModifyForm::EmployeeAddModifyForm(QWidget *parent, QList<int> employeePositionIDList,
                                             QStringList employeePositionList, Employee oldEmployee) :
    QDialog(parent),
    ui(new Ui::EmployeeAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(employeePositionIDList, employeePositionList);

    this->oldEmployee = oldEmployee;

    ui->lblId->setText(QString::number(oldEmployee.getId()));
    ui->ledFirstName->setText(oldEmployee.getFirstName());
    ui->ledMiddleName->setText(oldEmployee.getMiddleName());
    ui->ledLastName->setText(oldEmployee.getLastName());
    ui->ledUsername->setText(oldEmployee.getUsername());
    ui->ledSSN->setText(oldEmployee.getSsn());
    ui->cbxEmployeePosition->setCurrentText(oldEmployee.getPosition().getName());
    ui->dedDob->setDate(oldEmployee.getDob());
    ui->cbxGender->setCurrentText(oldEmployee.getGender() == "M" ? "Male" : "Female");
    ui->ledEmail->setText(oldEmployee.getEmail());
    ui->ledPhone->setText(oldEmployee.getPhone());
    ui->ledStreetAddress->setText(oldEmployee.getStreetAddress());
    ui->ledCity->setText(oldEmployee.getCity());
    ui->cbxStateCode->setCurrentText(oldEmployee.getStateCode());
    ui->ledZipcode->setText(oldEmployee.getZipcode());
    ui->dsbSalary->setValue(oldEmployee.getSalary());

    // Password doesn't have to be edited
    ui->ledPassword->setEnabled(false);

    queryType = "update";
    this->setWindowTitle("Update Employee");
}

EmployeeAddModifyForm::~EmployeeAddModifyForm()
{
    delete ui->ledZipcode->validator();
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initializes some features of form.
// @param QList<int> employeePositionIDList. List with Ids of employee positions.
// @param QStringList employeePositionList. List with employee positions.
void EmployeeAddModifyForm::initializeForm(QList<int> employeePositionIDList, QStringList employeePositionList)
{
    // Prevent user from resizing Dialog.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Delete employee position "Any".
    employeePositionList.removeAt(0);
    ui->cbxEmployeePosition->addItems(employeePositionList);
    this->employeePositionIDList = employeePositionIDList;
    this->employeePositionList = employeePositionList;

    ui->cbxGender->addItems({ "Male", "Female" });

    QStringList stateCodeList = {
        "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "DC", "FL", "GA", "HI", "ID",
        "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE",
        "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "PR", "RI", "SC", "SC",
        "SD", "TN", "TX", "UT", "VT", "VA", "VI", "WA", "WV", "WI", "WY"
    };
    ui->cbxStateCode->addItems(stateCodeList);

    // Makes ledZipcode to only accept 5 digits
    QRegExp rx("\\d{5}");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->ledZipcode->setValidator(validator);

    // Emits signal when lineEdit is focussed in or out.
    connect (ui->ledPhone, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledPhone_focussed(bool)));
    connect (ui->ledSSN, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledSSN_focussed(bool)));

    ui->ledSSN->setPlaceholderText("999-999-9999");
    ui->ledPhone->setPlaceholderText("01 (999) 999-9999");
}

// Checks if data input is correct.
// @return bool. True if data is correct.
bool EmployeeAddModifyForm::validateFields()
{
    // Error message
    QString message = "Invalid input:";

    // SSN format
    QRegExp rx("\\d{3}-\\d{3}-\\d{4}|\\d{0}");
    QRegExpValidator v(rx, 0);
    // Position where validation check is gonna start from
    int pos = 0;

    // Email format
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
    rx.setPattern("\\d{5}");
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

// Checks if all required fields were filled out.
// @return bool. True if data if they where.
bool EmployeeAddModifyForm::areFieldsNotBlank()
{
    // Error message
    QString message = "Please fill following fields:";

    if(ui->ledFirstName->text() == "")
        message += ", First name";
    if(ui->ledLastName->text() == "")
        message += ", Last name";
    if(ui->ledUsername->text() == "")
        message += ", Username";
    // ui->chkPassword with value "True" means that user wants to edit password
    if(ui->ledPassword->text() == "" && ui->chkPassword->checkState())
        message += ", Password";
    if(ui->ledSSN->text() == "")
        message += ", SSN";
    if(ui->ledStreetAddress->text() == "")
        message += ", Street address";
    if(ui->ledCity->text() == "")
        message += ", City";
    if(ui->ledZipcode->text() == "")
        message += ", Zipcode";
    if(ui->dsbSalary->value() == 0.00f)
        message += ", Salary";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Sets the text of update query.
// @param QString* queryText. Holds text of update query.
// @return bool. True if text was set successfully.
bool EmployeeAddModifyForm::setUpdateQueryText(QString *queryText)
{
    *queryText = "UPDATE employee ";
    QStringList setsList;

    if(oldEmployee.getFirstName() != newEmployee.getFirstName())
        setsList.append(", first_name = :first_name ");
    if(oldEmployee.getMiddleName() != newEmployee.getMiddleName())
        setsList.append(", middle_name = :middle_name ");
    if(oldEmployee.getLastName() != newEmployee.getLastName())
        setsList.append(", last_name = :last_name ");
    if(oldEmployee.getUsername() != newEmployee.getUsername())
        setsList.append(", username = :username ");
    if(ui->chkPassword->checkState())
        setsList.append(", password = :password ");
    if(oldEmployee.getSsn() != newEmployee.getSsn())
        setsList.append(", ssn = :ssn ");
    if(oldEmployee.getPosition().getId() != newEmployee.getPosition().getId())
        setsList.append(", employee_position_id = :employee_position_id ");
    if(oldEmployee.getDob() != newEmployee.getDob())
        setsList.append(", date_of_birth = :date_of_birth ");
    if(oldEmployee.getGender() != newEmployee.getGender())
        setsList.append(", gender = :gender ");
    if(oldEmployee.getEmail() != newEmployee.getEmail())
        setsList.append(", email = :email ");
    if(oldEmployee.getPhone() != newEmployee.getPhone())
        setsList.append(", phone = :phone ");
    if(oldEmployee.getStreetAddress() != newEmployee.getStreetAddress())
        setsList.append(", street_address = :street_address ");
    if(oldEmployee.getCity() != newEmployee.getCity())
        setsList.append(", city = :city ");
    if(oldEmployee.getStateCode() != newEmployee.getStateCode())
        setsList.append(", state_code = :state_code ");
    if(oldEmployee.getZipcode() != newEmployee.getZipcode())
        setsList.append(", zipcode = :zipcode ");
    if(oldEmployee.getSalary() != newEmployee.getSalary())
        setsList.append(", salary = :salary ");

    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE employee_id = :employee_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void EmployeeAddModifyForm::on_pbtOk_clicked()
{
    // Return if required fields weren't filled out or they have incorrect data
    if(!areFieldsNotBlank() || !validateFields())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();

    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of employee position?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Gets rid of "-" from SSN
    QString ssn = ui->ledSSN->text();
    ssn.replace(QString("-"), QString(""));

    // Gets rid of "-", "(" and ")" from phone number
    QString phone = ui->ledPhone->text();
    phone.replace(QString("-"), QString(""));
    phone.replace(QString("("), QString(""));
    phone.replace(QString(")"), QString(""));

    newEmployee.setId(ui->lblId->text().toInt());
    newEmployee.setFirstName(ui->ledFirstName->text());
    newEmployee.setMiddleName(ui->ledMiddleName->text());
    newEmployee.setLastName(ui->ledLastName->text());
    newEmployee.setUsername(ui->ledUsername->text());
    newEmployee.setPassword(ui->ledPassword->text());
    newEmployee.setSsn(ssn);
    newEmployee.setPosition(EmployeePosition(employeePositionIDList.at(ui->cbxEmployeePosition->currentIndex()),
                                             employeePositionList.at(ui->cbxEmployeePosition->currentIndex())));
    newEmployee.setDob(ui->dedDob->date());
    newEmployee.setGender(ui->cbxGender->currentText() == "Male" ? 'M' : 'F');
    newEmployee.setEmail(ui->ledEmail->text());
    newEmployee.setPhone(phone);
    newEmployee.setStreetAddress(ui->ledStreetAddress->text());
    newEmployee.setCity(ui->ledCity->text());
    newEmployee.setStateCode(ui->cbxStateCode->currentText());
    newEmployee.setZipcode(ui->ledZipcode->text());
    newEmployee.setSalary(ui->dsbSalary->value());
    newEmployee.setHiringDate(QDate::currentDate());

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
        queryText = "INSERT INTO employee (first_name, middle_name, last_name, username, password, ssn, employee_position_id, "
                    "date_of_birth, gender, email, phone, street_address, city, state_code, zipcode, salary, hiring_date) "
                    "VALUES (:first_name, :middle_name, :last_name, :username, :password, :ssn, :employee_position_id, "
                    ":date_of_birth, :gender, :email, :phone, :street_address, :city, :state_code, :zipcode, :salary, :hiring_date);";
    else
        if(!setUpdateQueryText(&queryText))
            return;

    query.prepare(queryText);

    query.bindValue(":employee_id", newEmployee.getId());
    query.bindValue(":first_name", newEmployee.getFirstName());
    query.bindValue(":middle_name", newEmployee.getMiddleName());
    query.bindValue(":last_name", newEmployee.getLastName());
    query.bindValue(":username", newEmployee.getUsername());
    query.bindValue(":password", newEmployee.getPassword());
    query.bindValue(":ssn", newEmployee.getSsn());
    query.bindValue(":employee_position_id", newEmployee.getPosition().getId());
    query.bindValue(":date_of_birth", newEmployee.getDob().toString("yyyy-M-d"));
    query.bindValue(":gender", newEmployee.getGender());
    query.bindValue(":email", newEmployee.getEmail());
    query.bindValue(":phone", newEmployee.getPhone());
    query.bindValue(":street_address", newEmployee.getStreetAddress());
    query.bindValue(":city", newEmployee.getCity());
    query.bindValue(":state_code", newEmployee.getStateCode());
    query.bindValue(":zipcode", newEmployee.getZipcode());
    query.bindValue(":salary", newEmployee.getSalary());
    query.bindValue(":hiring_date", newEmployee.getHiringDate().toString("yyyy-M-d"));

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Employee \"" + newEmployee.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Employee \"" + newEmployee.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Employee \"" + newEmployee.getFullName() + "\" " + queryType + " successful!");
    // Emit signal that means push button pbtOk was clicked.
    emit is_pbtOK_clicked(true);
    close();

}

void EmployeeAddModifyForm::on_pbtCancel_clicked()
{
    // Emit signal that means push button pbtOk wasn't clicked.
    emit is_pbtOK_clicked(false);
    close();
}

void EmployeeAddModifyForm::on_chkPassword_stateChanged(int checked)
{
    ui->ledPassword->setEnabled(checked);
}

// Executes when ledPhone is foccussed in or out.
// @param bool hasFocus. True if it was focussed in. False if focussed out.
void EmployeeAddModifyForm::on_ledPhone_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledPhone->setInputMask("dd (999) 999-9999");
    else
        // If after focussing out the user leave it blank, delete input mask
        if( ui->ledPhone->text() == " () -")
            ui->ledPhone->setInputMask("");
}

// Executes when ledSSN is foccussed in or out.
// @param bool hasFocus. True if it was focussed in. False if focussed out.
void EmployeeAddModifyForm::on_ledSSN_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledSSN->setInputMask("999-999-9999");
    else
        // If after focussing out the user leave it blank, delete input mask
        if( ui->ledSSN->text() == "--")
            ui->ledSSN->setInputMask("");
}
