/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows add new or update existing promotion type.

*/

#include "promotiontypeaddmodifyform.h"
#include "ui_promotiontypeaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new promotion type
PromotionTypeAddModifyForm::PromotionTypeAddModifyForm(QWidget *parent, QList<QList<QString>> customerTypesList, QList<QList<QString>> theaterTypesList,
                                                       QList<QList<QString>> paymentTypesList) :
    QDialog(parent),
    ui(new Ui::PromotionTypeAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(customerTypesList, theaterTypesList, paymentTypesList);

    ui->lblId->hide();
    ui->lbllblId->hide();
    this->setWindowTitle("New Promotion Type");

    queryType = "insertion";
}

// Constructor for updating existing promotion type
PromotionTypeAddModifyForm::PromotionTypeAddModifyForm(QWidget *parent, QList<QList<QString>> customerTypesList, QList<QList<QString>> theaterTypesList,
                                                       QList<QList<QString>> paymentTypesList, PromotionType oldPromotionType) :
    QDialog(parent),
    ui(new Ui::PromotionTypeAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(customerTypesList, theaterTypesList, paymentTypesList);

    this->oldPromotionType = oldPromotionType;

    ui->lblId->setText(QString::number(oldPromotionType.getId()));
    ui->ledName->setText(oldPromotionType.getName());
    ui->spbDiscount->setValue(oldPromotionType.getDiscount());
    ui->tedDescription->setPlainText(oldPromotionType.getDescription());
    ui->cbxAgeCategory->setCurrentText(capitalize(oldPromotionType.getAgeCategory()));
    ui->cbxCustomerType->setCurrentText(oldPromotionType.getCustomerType().getName());
    ui->cbxTheaterType->setCurrentText(oldPromotionType.getTheaterType().getName());
    ui->cbxPaymentType->setCurrentText(oldPromotionType.getPaymentType().getName());

   if(oldPromotionType.getMonday())
       ui->lwdDays->item(0)->setCheckState(Qt::Checked);
   if(oldPromotionType.getTuesday())
       ui->lwdDays->item(1)->setCheckState(Qt::Checked);
   if(oldPromotionType.getWednesday())
       ui->lwdDays->item(2)->setCheckState(Qt::Checked);
   if(oldPromotionType.getThursday())
       ui->lwdDays->item(3)->setCheckState(Qt::Checked);
   if(oldPromotionType.getFriday())
       ui->lwdDays->item(4)->setCheckState(Qt::Checked);
   if(oldPromotionType.getSaturday())
       ui->lwdDays->item(5)->setCheckState(Qt::Checked);
   if(oldPromotionType.getSunday())
       ui->lwdDays->item(6)->setCheckState(Qt::Checked);

   this->setWindowTitle("Update Promotion Type");

    queryType = "update";
}

PromotionTypeAddModifyForm::~PromotionTypeAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Capitalize QString text.
// @param const QString &str. QString that's gonna be capitalized.
// @return QString. Capitalized QString.
QString PromotionTypeAddModifyForm::capitalize(const QString &str)
{
    QString tmp = str;
    // if you want to ensure all other letters are lowercase:
    //tmp = tmp.toLower();
    tmp[0] = str[0].toUpper();
    return tmp;
}

// Initialize some features of form.
// @param QList<QList<QString>> customerTypesList. 2D QList that contains Id and customer type.
// @param QList<QList<QString>> theaterTypesList. 2D QList that contains Id and theater type.
// @param QList<QList<QString>> paymentTypesList. 2D QList that contains Id and payment type.
void PromotionTypeAddModifyForm::initializeForm(QList<QList<QString>> customerTypesList, QList<QList<QString>> theaterTypesList,
                                                QList<QList<QString>> paymentTypesList)
{
    // Prevent user from resizing window.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Remove "Any" item
    customerTypesList.removeAt(0);
    this->customerTypesList = customerTypesList;
    for(int i = 0; i < customerTypesList.length(); i++)
    {
        ui->cbxCustomerType->addItem(customerTypesList[i][1]);
    }

    // Remove "Any" item
    theaterTypesList.removeAt(0);
    this->theaterTypesList = theaterTypesList;
    for(int i = 0; i < theaterTypesList.length(); i++)
    {
        ui->cbxTheaterType->addItem(theaterTypesList[i][1]);
    }

    // Remove "Any" item
    paymentTypesList.removeAt(0);
    this->paymentTypesList = paymentTypesList;
    for(int i = 0; i < paymentTypesList.length(); i++)
    {
        ui->cbxPaymentType->addItem(paymentTypesList[i][1]);
    }
    ui->cbxPaymentType->setCurrentText("No type");

    QStringList ageCategoryList = { "X", "Adult", "Kid", "Senior" };
    ui->cbxAgeCategory->addItems(ageCategoryList);

    QStringList days =
    {
        "Monday", "Tuesday", "Wednesday",
        "Thursday", "Friday", "Saturday", "Sunday"
    };

    // Add items to lwdDays and make them checkable.
    for(int i = 0; i < days.length(); i++)
    {
        ui->lwdDays->addItem(days.at(i));
        QListWidgetItem* item = ui->lwdDays->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    // Executes highlightChecked slot when item in lwdDays changes state.
    QObject::connect(ui->lwdDays, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));
}

// Checks that required fields were filled out.
// @return bool. True if all required fields were filled out.
bool PromotionTypeAddModifyForm::areFieldsNotBlank()
{
    // Error message
    QString message = "Please fill following fields:";

    if(ui->ledName->text() == "")
        message += ", Name";
    if(ui->spbDiscount->value() == 0)
        message += ", Discount";

    // Value true when at least 1 item was checked in lwdDays
    bool isItemChecked = false;
    for(int i = 0; i < ui->lwdDays->count(); i++)
        if(ui->lwdDays->item(i)->checkState() == Qt::Checked)
        {
            isItemChecked = true;
            break;
        }

    if(!isItemChecked)
        message += ", Days";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Sets text of update query text.
// @param QString *queryText. Holds text of update query.
// @return bool. True if at least 1 fields wants to be updated.
bool PromotionTypeAddModifyForm::setUpdateQueryText(QString *queryText)
{
    *queryText = "UPDATE promotion_type ";
    QStringList setsList;

    if(oldPromotionType.getName() != newPromotionType.getName())
        setsList.append(", name = :name ");
    if(oldPromotionType.getDiscount() != newPromotionType.getDiscount())
        setsList.append(", discount = :discount ");
    if(oldPromotionType.getDescription() != newPromotionType.getDescription())
        setsList.append(", description = :description ");
    if(oldPromotionType.getAgeCategory() != newPromotionType.getAgeCategory())
        setsList.append(", age_category = :age_category ");
    if(oldPromotionType.getCustomerType().getName() != newPromotionType.getCustomerType().getName())
        setsList.append(", customer_type_id = :customer_type_id ");
    if(oldPromotionType.getTheaterType().getName() != newPromotionType.getTheaterType().getName())
        setsList.append(", theater_type_id = :theater_type_id ");
    if(oldPromotionType.getPaymentType().getName() != newPromotionType.getPaymentType().getName())
        setsList.append(", payment_type_id = :payment_type_id ");
    if(oldPromotionType.getMonday() != newPromotionType.getMonday())
        setsList.append(", monday = :monday ");
    if(oldPromotionType.getTuesday() != newPromotionType.getTuesday())
        setsList.append(", tuesday = :tuesday ");
    if(oldPromotionType.getWednesday() != newPromotionType.getWednesday())
        setsList.append(", wednesday = :wednesday ");
    if(oldPromotionType.getThursday() != newPromotionType.getThursday())
        setsList.append(", thursday = :thursday ");
    if(oldPromotionType.getFriday() != newPromotionType.getFriday())
        setsList.append(", friday = :friday ");
    if(oldPromotionType.getSaturday() != newPromotionType.getSaturday())
        setsList.append(", saturday = :saturday ");
    if(oldPromotionType.getSunday() != newPromotionType.getSunday())
        setsList.append(", sunday = :sunday ");

    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE promotion_type_id = :promotion_type_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

// Is executed when QListWidgetItem is checked or unchecked. Highlights checked items.
// @param QListWidgetItem *item. Item that was checked / unchecked.
void PromotionTypeAddModifyForm::highlightChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
        item->setBackgroundColor(QColor("#ffffb2"));
    else
        item->setBackgroundColor(QColor("#ffffff"));
}

void PromotionTypeAddModifyForm::on_pbtOk_clicked()
{
    // Checks if required fields were filled out. Return if not.
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of promotion type?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newPromotionType.setId(ui->lblId->text().toInt());
    newPromotionType.setName(ui->ledName->text());
    newPromotionType.setDiscount(ui->spbDiscount->value());
    newPromotionType.setDescription(ui->tedDescription->toPlainText());
    newPromotionType.setAgeCategory(
        ui->cbxAgeCategory->currentText() == "X"? "X" : ui->cbxAgeCategory->currentText().toLower()
    );
    newPromotionType.setCustomerType(
        CustomerType(customerTypesList[ui->cbxCustomerType->currentIndex()][0].toInt(),ui->cbxCustomerType->currentText(), 0, 0)
    );
    newPromotionType.setTheaterType(
        TheaterType(theaterTypesList[ui->cbxTheaterType->currentIndex()][0].toInt(),ui->cbxTheaterType->currentText(), 0, 0)
    );
    newPromotionType.setPaymentType(
        PaymentType(theaterTypesList[ui->cbxTheaterType->currentIndex()][0].toInt(),ui->cbxTheaterType->currentText())
    );
    newPromotionType.setMonday(ui->lwdDays->item(0)->checkState());
    newPromotionType.setTuesday(ui->lwdDays->item(1)->checkState());
    newPromotionType.setWednesday(ui->lwdDays->item(2)->checkState());
    newPromotionType.setThursday(ui->lwdDays->item(3)->checkState());
    newPromotionType.setFriday(ui->lwdDays->item(4)->checkState());
    newPromotionType.setSaturday(ui->lwdDays->item(5)->checkState());
    newPromotionType.setSunday(ui->lwdDays->item(6)->checkState());

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
        queryText = "INSERT INTO promotion_type (name, discount, description, age_category, customer_type_id, theater_type_id, "
                    "payment_type_id, monday, tuesday, wednesday, thursday, friday, saturday, sunday) "
                    "VALUES (:name, :discount, :description, :age_category, :customer_type_id, :theater_type_id, :payment_type_id, "
                    ":monday, :tuesday, :wednesday, :thursday, :friday, :saturday, :sunday);";
    }
    else
    {
        // Return if no fields were updated
        if(!setUpdateQueryText(&queryText))
            return;
    }

    query.prepare(queryText);

    query.bindValue(":promotion_type_id", newPromotionType.getId());
    query.bindValue(":name", newPromotionType.getName());
    query.bindValue(":discount", newPromotionType.getDiscount());
    query.bindValue(":description", newPromotionType.getDescription());
    query.bindValue(":age_category", newPromotionType.getAgeCategory());
    query.bindValue(":customer_type_id", newPromotionType.getCustomerType().getId());
    query.bindValue(":theater_type_id", newPromotionType.getTheaterType().getId());
    query.bindValue(":payment_type_id", newPromotionType.getPaymentType().getId());
    query.bindValue(":monday", newPromotionType.getMonday());
    query.bindValue(":tuesday", newPromotionType.getTuesday());
    query.bindValue(":wednesday", newPromotionType.getWednesday());
    query.bindValue(":thursday", newPromotionType.getThursday());
    query.bindValue(":friday", newPromotionType.getFriday());
    query.bindValue(":saturday", newPromotionType.getSaturday());
    query.bindValue(":sunday", newPromotionType.getSunday());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Promotion type \"" + newPromotionType.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Promotion type \"" + newPromotionType.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Promotion type \"" + newPromotionType.getName() + "\" " + queryType + " successful!");

    // Means pbtOk was clicked
    emit is_pbtOK_clicked(true);
    close();

}

void PromotionTypeAddModifyForm::on_pbtCancel_clicked()
{
    // Means pbtOk wasn't clicked
    emit is_pbtOK_clicked(false);
    close();
}

