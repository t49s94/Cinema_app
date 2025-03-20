/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user to add or update existing theater type.

*/

#include "theatertypeaddmodifyform.h"
#include "ui_theatertypeaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding a new theater type.
TheaterTypeAddModifyForm::TheaterTypeAddModifyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TheaterTypeAddModifyForm)
{
    ui->setupUi(this);
    // Prevent user from resizing Dialog
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lblId->hide();
    ui->lbllblId->hide();
    this->setWindowTitle("New Theater Type");
    queryType = "insertion";
}

// Constructor for updating existing theater type.
TheaterTypeAddModifyForm::TheaterTypeAddModifyForm(QWidget *parent, TheaterType oldTheaterType) :
    QDialog(parent),
    ui(new Ui::TheaterTypeAddModifyForm)
{
    ui->setupUi(this);
    // Prevent user from resizing Dialog
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->oldTheaterType = oldTheaterType;

    ui->lblId->setText(QString::number(oldTheaterType.getId()));
    ui->ledName->setText(oldTheaterType.getName());
    ui->dsbAdultPrice->setValue(oldTheaterType.getAdultPrice());
    ui->dsbKidPrice->setValue(oldTheaterType.getKidPrice());

    this->setWindowTitle("Update Theater Type");
    queryType = "update";
}

TheaterTypeAddModifyForm::~TheaterTypeAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Checks if required fields were filled out.
// @return bool. True if all required fields were filled out
bool TheaterTypeAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->ledName->text() == "")
        message += ", Name";
    if(ui->dsbAdultPrice->value() == 0.00f)
        message += ", Adult price";
    if(ui->dsbKidPrice->value() == 0.00f)
        message += ", Kid price";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Creates query text for update.
// @param QString* queryText. QString that contains update query text
// @return bool. False if there were no changes made.
bool TheaterTypeAddModifyForm::setUpdateQueryText(QString *queryText)
{
    *queryText = "UPDATE theater_type ";
    QStringList setsList;

    if(oldTheaterType.getName() != newTheaterType.getName())
        setsList.append(", name = :name ");
    if(oldTheaterType.getAdultPrice() != newTheaterType.getAdultPrice())
        setsList.append(", adult_price = :adult_price ");
    if(oldTheaterType.getKidPrice() != newTheaterType.getKidPrice())
        setsList.append(", kid_price = :kid_price ");

    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE theater_type_id = :theater_type_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void TheaterTypeAddModifyForm::on_pbtOk_clicked()
{
    // Return if there were required fields that weren't filled out
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of theater type?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newTheaterType.setId(ui->lblId->text().toInt());
    newTheaterType.setName(ui->ledName->text());
    newTheaterType.setAdultPrice(ui->dsbAdultPrice->value());
    newTheaterType.setKidPrice(ui->dsbKidPrice->value());

    QString queryText = "";

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    if(!conn.transaction())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    if(queryType == "insertion")
        queryText = "INSERT INTO theater_type (name, adult_price, kid_price) "
                    "VALUES (:name, :adult_price, :kid_price); ";
    else
        if(!setUpdateQueryText(&queryText))
            return;

    query.prepare(queryText);

    query.bindValue(":theater_type_id", newTheaterType.getId());
    query.bindValue(":name", newTheaterType.getName());
    query.bindValue(":adult_price", newTheaterType.getAdultPrice());
    query.bindValue(":kid_price", newTheaterType.getKidPrice());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Theater type \"" + newTheaterType.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Theater type \"" + newTheaterType.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Theater type \"" + newTheaterType.getName() + "\" " + queryType + " successful!");

    // Says that pbtOk was clicked
    emit is_pbtOK_clicked(true);
    close();

}

void TheaterTypeAddModifyForm::on_pbtCancel_clicked()
{
    // Says that pbtOk wasn't clicked
    emit is_pbtOK_clicked(false);
    close();
}
