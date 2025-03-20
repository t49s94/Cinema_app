#include "couponaddmodifyform.h"
#include "ui_couponaddmodifyform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window allows add or update existing coupon.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

// Constructor for adding new coupon
CouponAddModifyForm::CouponAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList) :
    QDialog(parent),
    ui(new Ui::CouponAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(promotionTypesList);

    ui->lblId->hide();
    ui->lbllblId->hide();
    this->setWindowTitle("New Coupon");

    queryType = "insertion";
}

// Constructor for updating existing coupon
CouponAddModifyForm::CouponAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList, Coupon oldCoupon) :
    QDialog(parent),
    ui(new Ui::CouponAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(promotionTypesList);

    this->oldCoupon = oldCoupon;

    ui->lblId->setText(QString::number(oldCoupon.getId()));
    ui->cbxPromotionType->setCurrentText(oldCoupon.getPromotionType().getName());
    ui->dedPrintDate->setDate(oldCoupon.getPrintDate());
    ui->dedExpirationDate->setDate(oldCoupon.getExpirationDate());

    this->setWindowTitle("Update Coupon");

    queryType = "update";
}

CouponAddModifyForm::~CouponAddModifyForm()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------------------------------------
// Functions

// Initialize some features of this form.
// @param QList<QList<QString>> promotionTypesList.
void CouponAddModifyForm::initializeForm(QList<QList<QString>> promotionTypesList)
{
    // Prevents user from resizing window
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    promotionTypesList.removeAt(0);
    this->promotionTypesList = promotionTypesList;

    for(int i = 0; i < promotionTypesList.length(); i++)
    {
        ui->cbxPromotionType->addItem(promotionTypesList[i][1]);
    }
}

// Creates query text for update.
// @param QString* queryText. QString that contains update query text
// @return bool. False if there were no changes made.
bool CouponAddModifyForm::setUpdateQueryText(QString* queryText)
{
    *queryText = "UPDATE coupon ";
    QStringList setsList;

    if(oldCoupon.getPromotionType().getId() != newCoupon.getPromotionType().getId())
        setsList.append(", promotion_type_id = :promotion_type_id ");
    if(oldCoupon.getPrintDate() != newCoupon.getPrintDate())
        setsList.append(", print_date = :print_date ");
    if(oldCoupon.getExpirationDate() != newCoupon.getExpirationDate())
        setsList.append(", expiration_date = :expiration_date ");


    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE coupon_id = :coupon_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}


void CouponAddModifyForm::on_pbtOk_clicked()
{
    QString action = queryType;
    action[0] = action[0].toUpper();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of coupon?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newCoupon.setId(ui->lblId->text().toInt());
    newCoupon.setPromotionType(
            PromotionType(promotionTypesList[ui->cbxPromotionType->currentIndex()][0].toInt(), ui->cbxPromotionType->currentText(), 0,
            "", "", CustomerType(), TheaterType(), PaymentType(), 0, 0, 0, 0, 0, 0, 0)
    );
    newCoupon.setPrintDate(ui->dedPrintDate->date());
    newCoupon.setExpirationDate(ui->dedExpirationDate->date());
    newCoupon.setUsed(0);

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
        queryText = "INSERT INTO coupon (promotion_type_id, print_date, expiration_date, used) "
                    "VALUES (:promotion_type_id, :print_date, :expiration_date, 0); ";
    }
    else
    {
        // If there's an error, return
        if(!setUpdateQueryText(&queryText))
            return;
    }

    query.prepare(queryText);

    query.bindValue(":coupon_id", newCoupon.getId());
    query.bindValue(":promotion_type_id", newCoupon.getPromotionType().getId());
    query.bindValue(":print_date", newCoupon.getPrintDate().toString("yyyy-M-d"));
    query.bindValue(":expiration_date", newCoupon.getExpirationDate().toString("yyyy-M-d"));

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Coupon " +  queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Coupon " +  queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Coupon " +  queryType + " successful!");
    emit is_pbtOK_clicked(true);
    close();

}

void CouponAddModifyForm::on_pbtCancel_clicked()
{
    emit is_pbtOK_clicked(false);
    close();
}

