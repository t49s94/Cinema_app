/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays a table with customers information. Allows to look up, add new, update or delete customer.

*/

#include "customersform.h"
#include "ui_customersform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

CustomersForm::CustomersForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomersForm)
{
    ui->setupUi(this);
    // Prevents user from resizing window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledFirstName->setPlaceholderText("Any");
    ui->ledMiddleName->setPlaceholderText("Any");
    ui->ledLastName->setPlaceholderText("Any");
    ui->ledUsername->setPlaceholderText("Any");
    ui->ledMemberCardId->setPlaceholderText("Any");
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
    ui->dedSinceTo->setDate(QDate::currentDate());
    ui->dedPrintDateTo->setDate(QDate::currentDate());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load customer types!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM customer_type;");

    customerTypeList.append("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load customer types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        customerTypeIDList.append(query.value(0).toInt());
        customerTypeList.append(query.value(1).toString());
    }

    ui->cbxCustomerType->addItems(customerTypeList);

    conn.connClose();

    // Makes ledZipcode to only accept 5 digits.
    QRegExp rx("\\d{5}");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->ledZipcode->setValidator(validator);

    // Executes slot when ledPhone is focussed in or out.
    connect (ui->ledPhone, SIGNAL (MyLineEdit::focussed()), this, SLOT(on_ledPhone_focussed(bool)));
}

CustomersForm::~CustomersForm()
{
    if (model != 0)
        delete model;
    delete ui->tvwTable->model();
    delete ui->ledZipcode->validator();
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with customer's information.
// @return bool. True if table was sucessfully loaded.
bool CustomersForm::loadTable()
{
    // Checks if fields have correct data.
    if(!validateFields())
        return false;

    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT c.customer_id AS ID, c.first_name AS 'First name', c.middle_name AS 'Middle name', c.last_name AS 'Last name', "
                "c.username AS Username, ct.name AS 'Customer type', mc.member_card_id AS 'Member card ID', mc.points AS Points, "
                "mc.current_year_visits AS 'Current year visits', mc.print_date AS 'Print date', c.date_of_birth AS 'Date of birth', c.gender AS Gender, "
                "c.email AS Email, c.phone AS Phone, c.street_address AS 'Street address', c.city AS City, "
                "c.state_code AS 'State code', c.zipcode AS Zipcode, c.since AS Since "
                "FROM customer c "
                "INNER JOIN customer_type ct "
                "ON c.customer_type_id = ct.customer_type_id "
                "INNER JOIN member_card mc "
                "ON c.member_card_id = mc.member_card_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Get values from fields and append them to query Text

    QString firstName = ui->ledFirstName->text();
    queryText += firstName != ""? " AND c.first_name = :firstName" : "";

    QString middleName = ui->ledMiddleName->text();
    queryText += middleName != ""? " AND c.middle_name = :middleName" : "";

    QString lastName = ui->ledLastName->text();
    queryText += lastName != ""? " AND c.last_name = :lastName" : "";

    QString username = ui->ledUsername->text();
    queryText += username != ""? " AND c.username = :username" : "";

    QString customerType = ui->cbxCustomerType->currentText();
    queryText += customerType != "Any"? " AND ct.name = :customerType" : "";

    QString memberCardId = ui->ledMemberCardId->text();
    queryText += memberCardId != ""? " AND mc.member_card_id = :memberCardId" : "";

    QString pointsFrom = ui->spbPointsFrom->text();
    queryText += pointsFrom != ""? " AND mc.points >= :pointsFrom" : "";

    QString pointsTo = ui->spbPointsTo->text();
    queryText += pointsTo != ""? " AND mc.points <= :pointsTo" : "";

    QString visitsFrom = ui->spbVisitsFrom->text();
    queryText += visitsFrom != ""? " AND mc.current_year_visits >= :visitsFrom" : "";

    QString visitsTo = ui->spbVisitsTo->text();
    queryText += visitsTo != ""? " AND mc.current_year_visits <= :visitsTo" : "";

    QString printDateFrom = ui->dedPrintDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkPrintDateFrom->isChecked()? " AND mc.print_date >= :printDateFrom" : "";

    QString printDateTo = ui->dedPrintDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkPrintDateTo->isChecked()? " AND mc.print_date <= :printDateTo" : "";

    QString dobFrom = ui->dedDobFrom->date().toString("yyyy-M-d");
    queryText += ui->chkDobFrom->isChecked()? " AND c.date_of_birth >= :dobFrom" : "";

    QString dobTo = ui->dedDobTo->date().toString("yyyy-M-d");
    queryText += ui->chkDobTo->isChecked()? " AND c.date_of_birth <= :dobTo" : "";

    QString email = ui->ledEmail->text();
    queryText += email != ""? " AND c.email = :email" : "";

    QString phone = ui->ledPhone->text();
    phone.replace(QString("-"), QString(""));
    phone.replace(QString("("), QString(""));
    phone.replace(QString(")"), QString(""));
    queryText += phone != ""? " AND c.phone = :phone" : "";

    QString streetAddress = ui->ledStreetAddress->text();
    queryText += streetAddress != ""? " AND c.street_address = :streetAddress" : "";

    QString city = ui->ledCity->text();
    queryText += city != ""? " AND c.city = :city" : "";

    QString stateCode = ui->cbxStateCode->currentText();
    queryText += stateCode != "Any"? " AND c.state_code = :stateCode" : "";

    QString zipcode = ui->ledZipcode->text();
    queryText += zipcode != ""? " AND c.zipcode = :zipcode" : "";

    QString sinceFrom = ui->dedSinceFrom->date().toString("yyyy-M-d");
    queryText += ui->chkSinceFrom->isChecked()? " AND c.since >= :sinceFrom" : "";

    QString sinceTo = ui->dedSinceTo->date().toString("yyyy-M-d");
    queryText += ui->chkSinceTo->isChecked()? " AND c.since <= :sinceTo" : "";

    QString gender = "";
    switch (ui->cbxGender->currentIndex()) {
        case 1:
            gender = "M";
            queryText += " AND c.gender = :gender";
            break;
        case 2:
            gender = "F";
            queryText +=" AND c.gender = :gender";
    }

    query.prepare( queryText + " ORDER BY c.customer_id;");

    query.bindValue(":firstName", firstName);
    query.bindValue(":middleName", middleName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":username", username);
    query.bindValue(":customerType", customerType);
    query.bindValue(":memberCardId", memberCardId);
    query.bindValue(":pointsFrom", pointsFrom);
    query.bindValue(":pointsTo", pointsTo);
    query.bindValue(":visitsFrom", visitsFrom);
    query.bindValue(":visitsTo", visitsTo);
    query.bindValue(":printDateFrom", printDateFrom);
    query.bindValue(":printDateTo", printDateTo);
    query.bindValue(":dobFrom", dobFrom);
    query.bindValue(":dobTo", dobTo);
    query.bindValue(":email", email);
    query.bindValue(":phone", phone);
    query.bindValue(":streetAddress", streetAddress);
    query.bindValue(":city", city);
    query.bindValue(":stateCode", stateCode);
    query.bindValue(":zipcode", zipcode);
    query.bindValue(":sinceFrom", sinceFrom);
    query.bindValue(":sinceTo", sinceTo);
    query.bindValue(":gender", gender);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load customers!\nError: " + query.lastError().nativeErrorCode());
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

    conn.connClose();
    return true;

}

// Resets fields to default value.
void CustomersForm::resetFields()
{
    ui->ledFirstName->setText("");
    ui->ledMiddleName->setText("");
    ui->ledLastName->setText("");
    ui->ledUsername->setText("");
    ui->cbxCustomerType->setCurrentIndex(0);
    ui->ledMemberCardId->setText("");
    ui->spbPointsFrom->setValue(0);
    ui->spbPointsTo->setValue(0);
    ui->spbVisitsFrom->setValue(0);
    ui->spbVisitsTo->setValue(0);
    ui->chkPrintDateFrom->setCheckState(Qt::Unchecked);
    ui->chkPrintDateTo->setCheckState(Qt::Unchecked);
    ui->dedPrintDateTo->setDate(QDate::currentDate());
    ui->cbxGender->setCurrentIndex(0);
    ui->chkDobFrom->setCheckState(Qt::Unchecked);
    ui->chkDobTo->setCheckState(Qt::Unchecked);
    ui->dedDobTo->setDate(QDate::currentDate());
    ui->ledEmail->setText("");
    ui->ledPhone->setText("");
    ui->ledPhone->setInputMask("");
    ui->ledStreetAddress->setText("");
    ui->ledCity->setText("");
    ui->cbxStateCode->setCurrentIndex(0);
    ui->ledZipcode->setText("");
    ui->chkSinceFrom->setCheckState(Qt::Unchecked);
    ui->chkSinceTo->setCheckState(Qt::Unchecked);
    ui->dedSinceTo->setDate(QDate::currentDate());
    ui->cbxGender->setCurrentIndex(0);
}

// Checks if fields have correct data.
// @return bool. True if they do.
bool CustomersForm::validateFields()
{
    // Error message
    QString message = "Invalid input:";

    QRegExp rx("(\\d{3}-\\d{3}-\\d{4}|\\d{0}|--)");
    QRegExpValidator v(rx, 0);
    // Position where validation check will start from
    int pos = 0;

    // Email address format
    QRegExp mailRX("(\\b[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}\\b|d{0})");
    mailRX.setCaseSensitivity(Qt::CaseInsensitive);
    mailRX.setPatternSyntax(QRegExp::RegExp);

    QString email = ui->ledEmail->text();
    QString phone = ui->ledPhone->text();
    QString zipcode = ui->ledZipcode->text();

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

void CustomersForm::on_pbtLoadTable_clicked()
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

void CustomersForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void CustomersForm::on_pbtAddNew_clicked()
{
    CustomersAddModifyForm *addForm = new CustomersAddModifyForm(this, customerTypeIDList, customerTypeList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

// Deletes selected rows from table
void CustomersForm::on_pbtDelete_clicked()
{
    // Ask if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of customer(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Holds Ids of customers that are gonna be deleted
    QVector<int> costumerIds(select.count());
    // Holds Ids of member cards that are gonna be deleted
    QVector<int> memberCardIds(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate QVector<int> costumerIds and memberCardIds
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       costumerIds[i] = index.sibling(row,0).data().toInt();
       memberCardIds[i] = index.sibling(row,6).data().toInt();
   }

   // Says if there was an error during the deletion of customers and member cards
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete customer(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete customer(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete customers and member cards
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM costumer "
                     "WHERE costumer_id = :costumer_id;");
       query.bindValue(":costumer_id", costumerIds[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete customer(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

       query.prepare("DELETE FROM member_card "
                     "WHERE member_card_id = :member_card_id;");
       query.bindValue(":member_card_id", memberCardIds[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete customer(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", "Customer(s) deleted!");
       loadTable();
   }
}

// Update selected row from table
void CustomersForm::on_pbtModify_clicked()
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

    Customer c;
    c.setId(index.sibling(index.row(),0).data().toInt());
    c.setFirstName(index.sibling(index.row(),1).data().toString());
    c.setMiddleName(index.sibling(index.row(),2).data().toString());
    c.setLastName(index.sibling(index.row(),3).data().toString());
    c.setUsername(index.sibling(index.row(),4).data().toString());
    c.setCustomerType(CustomerType(index.sibling(index.row(),19).data().toInt(),
                                   index.sibling(index.row(),5).data().toString(), 0, 0));
    c.setMemberCard(MemberCard(index.sibling(index.row(),6).data().toInt(), 0, 0, QDate::currentDate()));
    c.setDob(index.sibling(index.row(),10).data().toString());
    c.setGender(index.sibling(index.row(),11).data().toChar());
    c.setEmail(index.sibling(index.row(),12).data().toString());
    c.setPhone(index.sibling(index.row(),13).data().toString());
    c.setStreetAddress(index.sibling(index.row(),14).data().toString());
    c.setCity(index.sibling(index.row(),15).data().toString());
    c.setStateCode(index.sibling(index.row(),16).data().toString());
    c.setZipcode(index.sibling(index.row(),17).data().toString());
    c.setSince(index.sibling(index.row(),18).data().toString());

    CustomersAddModifyForm *modifyForm = new CustomersAddModifyForm(this, customerTypeIDList, customerTypeList, c);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
    delete modifyForm;
}

// Executes when is_pbtOK_clicked signal is emitted from CustomersAddModifyForm.
// @param bool clicked. True if push button "pbtOK" was clicked
void CustomersForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

// Executes when ledPhone is foccussed in or out.
void CustomersForm::on_ledPhone_focussed(bool hasFocus)
{
    if(hasFocus)
        ui->ledPhone->setInputMask("dd (999) 999-9999");
    else
        // If after focussing out the user leave it blank, delete input mask
        if( ui->ledPhone->text() == " () -")
            ui->ledPhone->setInputMask("");
}

void CustomersForm::on_chkDobFrom_stateChanged(int checked)
{
    ui->dedDobFrom->setEnabled(checked);
}

void CustomersForm::on_chkDobTo_stateChanged(int checked)
{
    ui->dedDobTo->setEnabled(checked);
}

void CustomersForm::on_chkSinceFrom_stateChanged(int checked)
{
    ui->dedSinceFrom->setEnabled(checked);
}

void CustomersForm::on_chkSinceTo_stateChanged(int checked)
{
    ui->dedSinceTo->setEnabled(checked);
}

void CustomersForm::on_chkPrintDateFrom_stateChanged(int checked)
{
    ui->dedPrintDateFrom->setEnabled(checked);
}

void CustomersForm::on_chkPrintDateTo_stateChanged(int checked)
{
    ui->dedPrintDateTo->setEnabled(checked);
}
