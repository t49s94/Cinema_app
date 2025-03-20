#include "customersaddmodifyform.h"
#include "ui_customersaddmodifyform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window add or update existing customer.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

// Constructor for adding new customer
CustomersAddModifyForm::CustomersAddModifyForm(QWidget *parent, QList<int> customerTypeIDList, QStringList customerTypeList) :
    QDialog(parent),
    ui(new Ui::CustomersAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(customerTypeIDList, customerTypeList);

    ui->lblId->hide();
    ui->lbllblId->hide();
    ui->lblMemberCardId->hide();
    ui->lbllblMemberCardId->hide();
    ui->chkMemberCardId->hide();

    this->setWindowTitle("New Customer");

    queryType = "insertion";
}

// Constructor for updating existing customer
CustomersAddModifyForm::CustomersAddModifyForm(QWidget *parent, QList<int> customerTypeIDList, QStringList customerTypeList, Customer oldCustomer) :
    QDialog(parent),
    ui(new Ui::CustomersAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(customerTypeIDList, customerTypeList);

    this->oldCustomer = oldCustomer;

    ui->lblId->setText(QString::number(oldCustomer.getId()));
    ui->ledFirstName->setText(oldCustomer.getFirstName());
    ui->ledMiddleName->setText(oldCustomer.getMiddleName());
    ui->ledLastName->setText(oldCustomer.getLastName());
    ui->ledUsername->setText(oldCustomer.getUsername());
    ui->lblMemberCardId->setText(QString::number(oldCustomer.getMemberCard().getId()));
    ui->dedDob->setDate(oldCustomer.getDob());
    ui->cbxGender->setCurrentText(oldCustomer.getGender() == "M" ? "Male" : "Female");
    ui->ledEmail->setText(oldCustomer.getEmail());
    ui->ledPhone->setText(oldCustomer.getPhone());
    ui->ledStreetAddress->setText(oldCustomer.getStreetAddress());
    ui->ledCity->setText(oldCustomer.getCity());
    ui->cbxStateCode->setCurrentText(oldCustomer.getStateCode());
    ui->ledZipcode->setText(oldCustomer.getZipcode());

    this->setWindowTitle("Update Customer");

    queryType = "update";
}

CustomersAddModifyForm::~CustomersAddModifyForm()
{
    delete ui->ledZipcode->validator();
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initialize some features of this form.
// @param QList<int> customerTypeIDList. List with IDs of customer types.
// @param QStringList customerTypeList. List with names of customer types.
void CustomersAddModifyForm::initializeForm(QList<int> customerTypeIDList, QStringList customerTypeList)
{
    // Prevents user from resizing window.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    customerTypeList.removeAt(0);
    this->customerTypeIDList = customerTypeIDList;
    this->customerTypeList = customerTypeList;

    ui->cbxGender->addItems({ "Male", "Female" });

    QStringList stateCodeList = {
        "AL", "AK", "AZ", "AR", "CA", "CO", "CT", "DE", "DC", "FL", "GA", "HI", "ID",
        "IL", "IN", "IA", "KS", "KY", "LA", "ME", "MD", "MA", "MI", "MN", "MS", "MO", "MT", "NE",
        "NV", "NH", "NJ", "NM", "NY", "NC", "ND", "OH", "OK", "OR", "PA", "PR", "RI", "SC", "SC",
        "SD", "TN", "TX", "UT", "VT", "VA", "VI", "WA", "WV", "WI", "WY"
    };
    ui->cbxStateCode->addItems(stateCodeList);

    // Set validator to ledZipcode to only accept 5 digits
    QRegExp rx("\\d{5}");
    QValidator *ptrValidator = new QRegExpValidator(rx, this);
    ui->ledZipcode->setValidator(ptrValidator);

    // Execute slot when ledPhone is focus in or out.
    connect (ui->ledPhone, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledPhone_focussed(bool)));
    ui->ledPhone->setPlaceholderText("01 (999) 999-9999");
}

// Validates the fields in form.
// @return bool. True if fields have correct data
bool CustomersAddModifyForm::validateFields()
{
    // Error message to be displayed
    QString message = "Invalid input:";

    // Format
    QRegExp rx("\\d{3}-\\d{3}-\\d{4}|\\d{0}");
    // Validator
    QRegExpValidator v(rx, 0);
    // Position where validation check will start from
    int pos = 0;

    // Format for email addresses
    QRegExp mailRX("(\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b|d{0})");
    mailRX.setCaseSensitivity(Qt::CaseInsensitive);
    mailRX.setPatternSyntax(QRegExp::RegExp);

    QString email = ui->ledEmail->text();
    QString phone = ui->ledPhone->text();
    QString zipcode = ui->ledZipcode->text();

    if(!mailRX.exactMatch(email))
        message += ", email";

    // Format for phone numbers
    rx.setPattern("(\\d{0,2}\\s\\(\\d{3}\\)\\s\\d{3}-\\d{4}|d{0}|\\(\\)-)");
    v.setRegExp(rx);

    if(v.validate(phone, pos) != 2)
        message += ", phone";

    // Format for zipcodes
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

// Checks if required fields are not blank
// @return bool. True if all required fields were filled out.
bool CustomersAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->ledFirstName->text() == "")
        message += ", First name";
    if(ui->ledLastName->text() == "")
        message += ", Last name";
    if(ui->ledUsername->text() == "")
        message += ", Username";
    if(ui->ledEmail->text() == "")
        message += ", Email";
    if(ui->ledStreetAddress->text() == "")
        message += ", Street address";
    if(ui->ledCity->text() == "")
        message += ", City";
    if(ui->ledZipcode->text() == "")
        message += ", Zipcode";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Create a new member card for customer.
// @return bool. True if new card was succesfully created.
bool CustomersAddModifyForm::createNewMemberCard()
{
    QSqlQuery query;
    if(queryType == "insertion")
        query.prepare("INSERT INTO member_card (points, current_year_visits, print_date) "
                      "VALUES (0, 0, :printDate);");
    else
    {
        query.prepare("DELETE FROM member_card "
                      "WHERE member_card_id = :id");

        query.bindValue(":id", oldCustomer.getMemberCard().getId());

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Customer \"" + newCustomer.getFullName() + "\" " + queryType +
                                 " failed!\n Error: " + query.lastError().nativeErrorCode());
            return false;
        }

        query.prepare("INSERT INTO member_card (points, current_year_visits, print_date) "
                      "VALUES (:points, :current_year_visits, :printDate);");

    }

    query.bindValue(":points", oldCustomer.getMemberCard().getPoints());
    query.bindValue(":current_year_visits", oldCustomer.getMemberCard().getCurrentYearVisits());
    query.bindValue(":printDate", QDate::currentDate().toString("yyyy-M-d"));

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Customer \"" + newCustomer.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return false;
    }

    query.prepare("SELECT LAST_INSERT_ID();");
    query.exec();

    if(!query.next())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Cast member \"" + newCustomer.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return false;
    }

    if(queryType == "insertion")
        newCustomer.setMemberCard(MemberCard(query.value(0).toInt(), 0, 0, QDate::currentDate()));
    else
        newCustomer.setMemberCard(MemberCard(query.value(0).toInt(), oldCustomer.getMemberCard().getPoints(),
                                             oldCustomer.getMemberCard().getCurrentYearVisits(), QDate::currentDate()));

    return true;
}

// Sets the text of update query.
// @param QString* queryText. Holds the update query text.
// @return bool. False if there were no changes made in any field.
bool CustomersAddModifyForm::setUpdateQueryText(QString *queryText)
{
    if(ui->chkMemberCardId->checkState())
        if(!createNewMemberCard())
            return false;

    *queryText = "UPDATE customer ";
    // List with fields that will be updated
    QStringList setsList;

    if(oldCustomer.getFirstName() != newCustomer.getFirstName())
        setsList.append(", first_name = :first_name ");
    if(oldCustomer.getMiddleName() != newCustomer.getMiddleName())
        setsList.append(", middle_name = :middle_name ");
    if(oldCustomer.getLastName() != newCustomer.getLastName())
        setsList.append(", last_name = :last_name ");
    if(oldCustomer.getUsername() != newCustomer.getUsername())
        setsList.append(", username = :username ");
    if(ui->chkMemberCardId->checkState())
        setsList.append(", member_card_id = :member_card_id ");
    else
        newCustomer.setMemberCard(oldCustomer.getMemberCard());
    if(oldCustomer.getDob() != newCustomer.getDob())
        setsList.append(", date_of_birth = :date_of_birth ");
    if(oldCustomer.getGender() != newCustomer.getGender())
        setsList.append(", gender = :gender ");
    if(oldCustomer.getEmail() != newCustomer.getEmail())
        setsList.append(", email = :email ");
    if(oldCustomer.getPhone() != newCustomer.getPhone())
        setsList.append(", phone = :phone ");
    if(oldCustomer.getStreetAddress() != newCustomer.getStreetAddress())
        setsList.append(", street_address = :street_address ");
    if(oldCustomer.getCity() != newCustomer.getCity())
        setsList.append(", city = :city ");
    if(oldCustomer.getStateCode() != newCustomer.getStateCode())
        setsList.append(", state_code = :state_code ");
    if(oldCustomer.getZipcode() != newCustomer.getZipcode())
        setsList.append(", zipcode = :zipcode ");

    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    // Append update fields to query text
    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE customer_id = :customer_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void CustomersAddModifyForm::on_pbtOk_clicked()
{
    // Checks required fields are not blank and they have correct data
    if(!areFieldsNotBlank() || !validateFields())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    // Ask if user wants to proceed with process. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of employee position?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newCustomer.setId(ui->lblId->text().toInt());
    newCustomer.setFirstName(ui->ledFirstName->text());
    newCustomer.setMiddleName(ui->ledMiddleName->text());
    newCustomer.setLastName(ui->ledLastName->text());
    newCustomer.setUsername(ui->ledUsername->text());
    newCustomer.setDob(ui->dedDob->date());
    newCustomer.setGender(ui->cbxGender->currentText() == "Male" ? 'M' : 'F');
    newCustomer.setEmail(ui->ledEmail->text());
    newCustomer.setPhone(ui->ledPhone->text());
    newCustomer.setStreetAddress(ui->ledStreetAddress->text());
    newCustomer.setCity(ui->ledCity->text());
    newCustomer.setStateCode(ui->cbxStateCode->currentText());
    newCustomer.setZipcode(ui->ledZipcode->text());
    newCustomer.setSince(QDate::currentDate());

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
        if(!createNewMemberCard())
            return;

        newCustomer.setCustomerType(CustomerType(customerTypeIDList.at(1), customerTypeList.at(1), 0, 0));

        queryText = "INSERT INTO customer (first_name, middle_name, last_name, username, customer_type_id, member_card_id, "
                    "date_of_birth, gender, email, phone, street_address, city, state_code, zipcode, since) "
                    "VALUES (:first_name, :middle_name, :last_name, :username, :customer_type_id, :member_card_id, "
                    ":date_of_birth, :gender, :email, :phone, :street_address, :city, :state_code, :zipcode, :since);";
    }
    else
    {
        if(!setUpdateQueryText(&queryText))
            return;
    }

    query.prepare(queryText);

    query.bindValue(":customer_id", newCustomer.getId());
    query.bindValue(":first_name", newCustomer.getFirstName());
    query.bindValue(":middle_name", newCustomer.getMiddleName());
    query.bindValue(":last_name", newCustomer.getLastName());
    query.bindValue(":username", newCustomer.getUsername());
    query.bindValue(":customer_type_id", newCustomer.getCustomerType().getId());
    query.bindValue(":member_card_id", newCustomer.getMemberCard().getId());
    query.bindValue(":date_of_birth", newCustomer.getDob().toString("yyyy-M-d"));
    query.bindValue(":gender", newCustomer.getGender());
    query.bindValue(":email", newCustomer.getEmail());
    query.bindValue(":phone", newCustomer.getPhone());
    query.bindValue(":street_address", newCustomer.getStreetAddress());
    query.bindValue(":city", newCustomer.getCity());
    query.bindValue(":state_code", newCustomer.getStateCode());
    query.bindValue(":zipcode", newCustomer.getZipcode());
    query.bindValue(":since", newCustomer.getSince());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Customer \"" + newCustomer.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Customer \"" + newCustomer.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Customerr \"" + newCustomer.getFullName() + "\" " + queryType + " successful!");
    emit is_pbtOK_clicked(true);
    close();

}

void CustomersAddModifyForm::on_pbtCancel_clicked()
{
    // Says push button "pbtOk" wasn't clicked
    emit is_pbtOK_clicked(false);
    close();
}

// Executes when ledPhone is foccussed in or out.
void CustomersAddModifyForm::on_ledPhone_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledPhone->setInputMask("dd (999) 999-9999");
    else
        // If after focussing out the user leave it blank, delete input mask
        if( ui->ledPhone->text() == " () -")
            ui->ledPhone->setInputMask("");
}
