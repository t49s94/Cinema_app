/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays payment types. Allows look up, add new, delete or update existing payment type.

*/

#include "paymenttypesform.h"
#include "ui_paymenttypesform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

PaymentTypesForm::PaymentTypesForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PaymentTypesForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;
}

PaymentTypesForm::~PaymentTypesForm()
{
    delete ui->tvwTable->model();
    if(model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with information about payment types.
// @param bool. True if table was successfully loaded.
bool PaymentTypesForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load payment types!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT payment_type_id AS ID, name AS Name "
                  "FROM payment_type "
                  "ORDER BY payment_type_id;");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load payment types!\nError: " + query.lastError().nativeErrorCode());
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

    conn.connClose();
    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void PaymentTypesForm::on_pbtLoadTable_clicked()
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

// Delete selected payment types from table
void PaymentTypesForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of payment type(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Vector with ids of payment types to be deleted
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

   // Says if there was an error during deletion of payment types.
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete payment type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete payment type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete payment types.
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;
       query.prepare("DELETE FROM payment_type "
                     "WHERE payment_type_id = :id;");
       query.bindValue(":id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete payment type(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }
   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete payment type(s)!\nError: " + conn.nativeErrorCode());
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Payment type(s) deleted!");
       loadTable();

   }

}

void PaymentTypesForm::on_pbtAddNew_clicked()
{
    PaymentTypeAddModifyForm *addForm = new PaymentTypeAddModifyForm(this);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Update selected payment type from table.
void PaymentTypesForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    PaymentType pt;
    pt.setId(index.sibling(index.row(),0).data().toInt());
    pt.setName(index.sibling(index.row(),1).data().toString());

    PaymentTypeAddModifyForm *modifyForm = new PaymentTypeAddModifyForm(this, pt);
    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
}

// Executed when is_pbtOK_clicked signal from PaymentTypeAddModifyForm was emitted.
// @param bool clicked. True if push button pbtOk was clicked.
void PaymentTypesForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}


