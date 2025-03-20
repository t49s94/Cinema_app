#include "customertypeform.h"
#include "ui_customertypeform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays the customer types stored in DB. Allows to look up, add new, delete or update.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

CustomerTypeForm::CustomerTypeForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CustomerTypeForm)
{
    ui->setupUi(this);
    model = 0;
    //Prevent user from resizing window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

}

CustomerTypeForm::~CustomerTypeForm()
{
    delete ui->tvwTable->model();
    if (model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Loads table with information about customer types.
// @return bool. True if table was sucessfully loaded.
bool CustomerTypeForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load customer types!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT customer_type_id AS ID, name AS Name, cumulation AS 'Cumulation (%)', visits_required as 'Visits required' "
                  "FROM customer_type "
                  "ORDER BY customer_type_id;");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load customer types!\nError: " + query.lastError().nativeErrorCode());
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

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void CustomerTypeForm::on_pbtLoadTable_clicked()
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

// Delete selected rows from table
void CustomerTypeForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of customer type(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // QVector that contains the ids of customer types that are gonna be deleted
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

   // Says if an error occured while deleting customer types.
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete customer type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete customer type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete customer types
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;
       query.prepare("DELETE FROM customer_type "
                     "WHERE customer_type_id = :id;");
       query.bindValue(":id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete customer type(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }
   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete customer type(s)!\nError: " + conn.nativeErrorCode());
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Cast position(s) deleted!");
       loadTable();

   }

}

void CustomerTypeForm::on_pbtAddNew_clicked()
{
    CustomerTypesAddModifyForm *addForm = new CustomerTypesAddModifyForm(this);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

// Update selected row.
void CustomerTypeForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() == 1)
    {
        QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

        CustomerType ct;
        ct.setId(index.sibling(index.row(),0).data().toInt());
        ct.setName(index.sibling(index.row(),1).data().toString());
        ct.setCumulation(index.sibling(index.row(),2).data().toInt());
        ct.setVisitsRequired(index.sibling(index.row(),3).data().toInt());

        CustomerTypesAddModifyForm *modifyForm = new CustomerTypesAddModifyForm(this, ct);
        connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
        modifyForm->exec();
        delete modifyForm;
    }
    else
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
    }
}

// Executes when is_pbtOK_clicked signal is emitted from CustomerTypesAddModifyForm.
// @param bool clicked. True if push button "pbtOK" was clicked.
void CustomerTypeForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
