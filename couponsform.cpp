/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays coupons stored in database. Allows add, delete or modify existing coupon.

*/

#include "couponsform.h"
#include "ui_couponsform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

CouponsForm::CouponsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CouponsForm)
{
    ui->setupUi(this);
    // Prevents user from resizing window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->dedPrintDateTo->setDate(QDate::currentDate());
    ui->dedExpirationDateTo->setDate(QDate::currentDate());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load data!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM promotion_type;");

    promotionTypesList.append({"0", "Any"});
    ui->cbxPromotionType->addItem("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load promotion types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        promotionTypesList.append({query.value(0).toString(), query.value(1).toString()});
        ui->cbxPromotionType->addItem(query.value(1).toString());
    }

    conn.connClose();

}

CouponsForm::~CouponsForm()
{
    delete ui->tvwTable->model();
    if (model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with information about coupons.
// @return bool. True if table was loaded succesfully.
bool CouponsForm::loadTable()
{
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load coupons!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT c.coupon_id AS ID, pt.name AS 'Promotion type', c.print_date AS 'Print date', c.expiration_date AS 'Expiration date', "
                "c.used AS Used, pt.promotion_type_id "
                "FROM coupon c "
                "INNER JOIN promotion_type pt "
                "ON c.promotion_type_id = pt.promotion_type_id "
                "WHERE 1 = 1 ";

    QString promotionType = ui->cbxPromotionType->currentText();
    queryText += promotionType != "Any"? " AND pt.name = :promotion_type" : "";

    QString printDateFrom = ui->dedPrintDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkPrintDateFrom->checkState() ? " AND c.print_date >= :print_date_from" : "";

    QString printDateTo = ui->dedPrintDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkPrintDateTo->checkState() ? " AND c.print_date <= :print_date_to" : "";

    QString expirationDateFrom = ui->dedExpirationDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkExpirationDateFrom->checkState() ? " AND c.expiration_date >= :expiration_date_from" : "";

    QString expirationDateTo = ui->dedExpirationDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkExpirationDateTo->checkState() ? " AND  c.expiration_date <= :expiration_date_to" : "";

    if (ui->rbtYes->isChecked())
         queryText += " AND c.used = 1";
    else if (ui->rbtNo->isChecked())
        queryText += " AND c.used = 0";

    query.prepare( queryText + " ORDER BY c.coupon_id;");

    query.bindValue(":promotion_type", promotionType);
    query.bindValue(":print_date_from", printDateFrom);
    query.bindValue(":print_date_to", printDateTo);
    query.bindValue(":expiration_date_from", expirationDateFrom);
    query.bindValue(":expiration_date_to", expirationDateTo);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load coupons!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

    // Model to filter data in table
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hides pt.promotion_type_id
    ui->tvwTable->hideColumn(5);

    conn.connClose();
    return true;

}

// Resets fields in form
void CouponsForm::resetFields()
{
    ui->cbxPromotionType->setCurrentIndex(0);
    ui->chkPrintDateFrom->setChecked(false);
    ui->chkPrintDateTo->setChecked(false);
    ui->chkExpirationDateFrom->setChecked(false);
    ui->chkExpirationDateTo->setChecked(false);
    ui->rbtNo->setChecked(false);
    ui->rbtYes->setChecked(false);
    ui->dedPrintDateFrom->setDate(QDate(2000, 1, 1));
    ui->dedPrintDateTo->setDate(QDate::currentDate());
    ui->dedExpirationDateFrom->setDate(QDate(2000, 1, 1));
    ui->dedExpirationDateTo->setDate(QDate::currentDate());
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void CouponsForm::on_pbtLoadTable_clicked()
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

void CouponsForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void CouponsForm::on_pbtAddNew_clicked()
{
    CouponAddModifyForm *addForm = new CouponAddModifyForm(this, promotionTypesList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

// Delete selected coupons
void CouponsForm::on_pbtDelete_clicked()
{
    // Ask if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of coupon(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected coupons.
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Vectors that contains the ID from selected coupons
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate Vector ids
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there was an error during the deletion of coupons
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete coupon(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete coupon(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete coupons
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM coupon "
                     "WHERE coupon_id = :coupon_id;");
       query.bindValue(":coupon_id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete coupon(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", "Coupon(s) deleted!");
       loadTable();
   }
}

// Modify selected coupon
void CouponsForm::on_pbtModify_clicked()
{
    // Make sure user only selected one record
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

    Coupon c;
    c.setId(index.sibling(index.row(),0).data().toInt());
    c.setPromotionType(
        PromotionType(index.sibling(index.row(),5).data().toInt(), index.sibling(index.row(),1).data().toString(),
                      0, "", "", CustomerType(), TheaterType(), PaymentType(), 0, 0, 0, 0, 0, 0, 0)
    );
    c.setPrintDate(index.sibling(index.row(),2).data().toDate());
    c.setExpirationDate(index.sibling(index.row(),3).data().toDate());
    c.setUsed(index.sibling(index.row(),4).data().toInt());

    CouponAddModifyForm *modifyForm = new CouponAddModifyForm(this, promotionTypesList, c);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
    delete modifyForm;

}

// Executed when push button "pbtOK" from CouponAddModifyForm is clicked
void CouponsForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

void CouponsForm::on_chkPrintDateFrom_stateChanged(int checked)
{
    ui->dedPrintDateFrom->setEnabled(checked);
}

void CouponsForm::on_chkPrintDateTo_stateChanged(int checked)
{
    ui->dedPrintDateTo->setEnabled(checked);
}

void CouponsForm::on_chkExpirationDateFrom_stateChanged(int checked)
{
    ui->dedExpirationDateFrom->setEnabled(checked);
}

void CouponsForm::on_chkExpirationDateTo_stateChanged(int checked)
{
    ui->dedExpirationDateTo->setEnabled(checked);
}
