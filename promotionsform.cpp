/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays promotions stored in DB. Allows look up, add new, delete or update existing promotion.

*/

#include "promotionsform.h"
#include "ui_promotionsform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

PromotionsForm::PromotionsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PromotionsForm)
{
    ui->setupUi(this);
    // Prevent user from resizing window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->dedStartDateTo->setDate(QDate::currentDate());
    ui->dedEndDateTo->setDate(QDate::currentDate());

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
        conn.connClose();
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

PromotionsForm::~PromotionsForm()
{
    delete ui->tvwTable->model();
    if(model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with information about promotions.
// @return bool. True if table was successfully loaded.
bool PromotionsForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load promotions!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT p.promotion_id AS ID, pt.name AS 'Promotion type', p.start_date AS 'Start date', p.end_date AS 'End date', "
                "pt.promotion_type_id "
                "FROM promotion p "
                "INNER JOIN promotion_type pt "
                "ON p.promotion_type_id = pt.promotion_type_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Append updated fields to update query text

    QString promotionType = ui->cbxPromotionType->currentText();
    queryText += promotionType != "Any"? " AND pt.name = :promotion_type" : "";

    QString startDateFrom = ui->dedStartDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkStartDateFrom->checkState() ? " AND p.start_date >= :start_date_from" : "";

    QString startDateTo = ui->dedStartDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkStartDateTo->checkState() ? " AND p.start_date <= :start_date_to" : "";

    QString endDateFrom = ui->dedEndDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkEndDateFrom->checkState() ? " AND p.end_date >= :end_date_from" : "";

    QString endDateTo = ui->dedEndDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkEndDateTo->checkState() ? " AND  p.end_date <= :end_date_to" : "";

    query.prepare( queryText + " ORDER BY p.promotion_id;");

    query.bindValue(":promotion_type", promotionType);
    query.bindValue(":start_date_from", startDateFrom);
    query.bindValue(":start_date_to", startDateTo);
    query.bindValue(":end_date_from", endDateFrom);
    query.bindValue(":end_date_to", endDateTo);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load promotions!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

    // Model that filters data in table
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hides pt.promotion_type_id
    ui->tvwTable->hideColumn(4);

    conn.connClose();
    return true;
}

// Reset fields to default value in form.
void PromotionsForm::resetFields()
{
    ui->cbxPromotionType->setCurrentIndex(0);
    ui->chkStartDateFrom->setChecked(false);
    ui->chkStartDateTo->setChecked(false);
    ui->chkEndDateFrom->setChecked(false);
    ui->chkEndDateTo->setChecked(false);
    ui->dedStartDateFrom->setDate(QDate(2000, 1, 1));
    ui->dedStartDateTo->setDate(QDate::currentDate());
    ui->dedEndDateFrom->setDate(QDate(2000, 1, 1));
    ui->dedEndDateTo->setDate(QDate::currentDate());
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void PromotionsForm::on_pbtLoadTable_clicked()
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

void PromotionsForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void PromotionsForm::on_pbtAddNew_clicked()
{
    PromotionAddModifyForm *addForm = new PromotionAddModifyForm(this, promotionTypesList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Deletes selected promotions from table
void PromotionsForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of promotion(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // List with Ids of promotions to be deleted.
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate QVector<int> ids
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there was an error duting deletion of promotions
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete promotion(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete promotion(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete promotions
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM promotion "
                     "WHERE promotion_id = :promotion_id;");
       query.bindValue(":promotion_id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete promotion(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", "Promotion(s) deleted!");
       loadTable();
   }
}

// Update selected promotions from table.
void PromotionsForm::on_pbtModify_clicked()
{
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

    Promotion p;
    p.setId(index.sibling(index.row(),0).data().toInt());
    p.setPromotionType(
        PromotionType(index.sibling(index.row(),4).data().toInt(), index.sibling(index.row(),1).data().toString(),
                      0, "", "", CustomerType(), TheaterType(), PaymentType(), 0, 0, 0, 0, 0, 0, 0)
    );
    p.setStartDate(index.sibling(index.row(),2).data().toDate());
    p.setEndDate(index.sibling(index.row(),3).data().toDate());

    PromotionAddModifyForm *modifyForm = new PromotionAddModifyForm(this, promotionTypesList, p);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();

}

// Is executed when signal is_pbtOK_clicked from PromotionAddModifyForm is emitted.
// @param bool clicked. True if push button "pbtOk" was clicked.
void PromotionsForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

void PromotionsForm::on_chkStartDateFrom_stateChanged(int checked)
{
    ui->dedStartDateFrom->setEnabled(checked);
}

void PromotionsForm::on_chkStartDateTo_stateChanged(int checked)
{
    ui->dedStartDateTo->setEnabled(checked);
}

void PromotionsForm::on_chkEndDateFrom_stateChanged(int checked)
{
    ui->dedEndDateFrom->setEnabled(checked);
}

void PromotionsForm::on_chkEndDateTo_stateChanged(int checked)
{
    ui->dedEndDateTo->setEnabled(checked);
}

