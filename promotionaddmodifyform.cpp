/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing promotion.

*/

#include "promotionaddmodifyform.h"
#include "ui_promotionaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new promotion
PromotionAddModifyForm::PromotionAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList) :
    QDialog(parent),
    ui(new Ui::PromotionAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(promotionTypesList);

    ui->lblId->hide();
    ui->lbllblId->hide();
    this->setWindowTitle("New Promotion");

    queryType = "insertion";
}

// Constructor for updating existing promotion.
PromotionAddModifyForm::PromotionAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList, Promotion oldPromotion) :
    QDialog(parent),
    ui(new Ui::PromotionAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(promotionTypesList);

    this->oldPromotion = oldPromotion;

    ui->lblId->setText(QString::number(oldPromotion.getId()));
    ui->cbxPromotionType->setCurrentText(oldPromotion.getPromotionType().getName());
    ui->dedStartDate->setDate(oldPromotion.getStartDate());
    ui->dedEndDate->setDate(oldPromotion.getEndDate());

    this->setWindowTitle("Update Promotion");

    queryType = "update";
}

PromotionAddModifyForm::~PromotionAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initialize some features in form.
// @param QList<QList<QString> > promotionTypesList. 2D List with Id and promotion type.
void PromotionAddModifyForm::initializeForm(QList<QList<QString> > promotionTypesList)
{
    // Prevent user from resizing window.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    promotionTypesList.removeAt(0);
    this->promotionTypesList = promotionTypesList;

    for(int i = 0; i < promotionTypesList.length(); i++)
        ui->cbxPromotionType->addItem(promotionTypesList[i][1]);
}

// Set the text of update query
// @param QString* queryText. Hold Query text.
// @return bool. True if at least 1 field will be updated
bool PromotionAddModifyForm::setUpdateQueryText(QString* queryText)
{
    *queryText = "UPDATE promotion ";
    QStringList setsList;

    if(oldPromotion.getPromotionType().getId() != newPromotion.getPromotionType().getId())
        setsList.append(", promotion_type_id = :promotion_type_id ");
    if(oldPromotion.getStartDate() != newPromotion.getStartDate())
        setsList.append(", start_date = :start_date ");
    if(oldPromotion.getEndDate() != newPromotion.getEndDate())
        setsList.append(", end_date = :end_date ");

    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE promotion_id = :promotion_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}

// Validates the fields in form.
// @return bool. True if fields have correct data
bool PromotionAddModifyForm::validateFields()
{
    if(ui->dedEndDate->date() < ui->dedStartDate->date())
    {
        QMessageBox::warning(this, "Error", "Invalid input: End date. End date must be same or later than Start date.");
        return false;
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void PromotionAddModifyForm::on_pbtOk_clicked()
{
    // Check if fields have correct data.
    if(!validateFields())
        return;

    // Asks if user wants to proceed. Return if not.
    QString action = queryType;
    action[0] = action[0].toUpper();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of promotion?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newPromotion.setId(ui->lblId->text().toInt());
    newPromotion.setPromotionType(
            PromotionType(promotionTypesList[ui->cbxPromotionType->currentIndex()][0].toInt(), ui->cbxPromotionType->currentText(), 0,
            "", "", CustomerType(), TheaterType(), PaymentType(), 0, 0, 0, 0, 0, 0, 0)
    );
    newPromotion.setStartDate(ui->dedStartDate->date());
    newPromotion.setEndDate(ui->dedEndDate->date());

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
        queryText = "INSERT INTO promotion (promotion_type_id, start_date, end_date) "
                    "VALUES (:promotion_type_id, :start_date, :end_date); ";
    }
    else
    {
        if(!setUpdateQueryText(&queryText))
            return;
    }

    query.prepare(queryText);

    query.bindValue(":promotion_id", newPromotion.getId());
    query.bindValue(":promotion_type_id", newPromotion.getPromotionType().getId());
    query.bindValue(":start_date", newPromotion.getStartDate().toString("yyyy-M-d"));
    query.bindValue(":end_date", newPromotion.getEndDate().toString("yyyy-M-d"));

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Promotion " +  queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Promotion " +  queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Promotion " +  queryType + " successful!");

    // Means pbtOk was clicked
    emit is_pbtOK_clicked(true);
    close();

}

void PromotionAddModifyForm::on_pbtCancel_clicked()
{
    // Means pbtOk wasn't clicked
    emit is_pbtOK_clicked(false);
    close();
}

