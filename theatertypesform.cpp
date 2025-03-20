/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays theater types stored in DB. Allows look up, add new, delete or update existing theater type.

*/

#include "theatertypesform.h"
#include "ui_theatertypesform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

TheaterTypesForm::TheaterTypesForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TheaterTypesForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;
}

TheaterTypesForm::~TheaterTypesForm()
{
    if(model != 0)
        delete model;
    delete ui->tvwTable->model();

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with information about theater types.
// @param bool. True if table was successfully loaded.
bool TheaterTypesForm::loadTable()
{
    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load theater types!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT theater_type_id AS ID, name AS Name, adult_price AS 'Adult price', kid_price 'Kid price' "
                  "FROM theater_type "
                  "ORDER BY theater_type_id;");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load theater types!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model = new QSqlQueryModel();
    model->setQuery(query);

    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
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

void TheaterTypesForm::on_pbtLoadTable_clicked()
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

// Delete selected theater types from table
void TheaterTypesForm::on_pbtDelete_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of theater type(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // QVector with Ids of theater types to be deleted
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

   // Says if there was an error during deletion
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete theater type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't delete theater type(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete theater types
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;
       query.prepare("DELETE FROM theater_type "
                     "WHERE theater_type_id = :id;");
       query.bindValue(":id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete theater type(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }
   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete theater type(s)!\nError: " + conn.nativeErrorCode());
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Theater type(s) deleted!");
       loadTable();

   }

}

void TheaterTypesForm::on_pbtAddNew_clicked()
{
    TheaterTypeAddModifyForm *addForm = new TheaterTypeAddModifyForm(this);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Update selected theater types from table
void TheaterTypesForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    TheaterType tt;
    tt.setId(index.sibling(index.row(),0).data().toInt());
    tt.setName(index.sibling(index.row(),1).data().toString());
    tt.setAdultPrice(index.sibling(index.row(),2).data().toDouble());
    tt.setKidPrice(index.sibling(index.row(),3).data().toDouble());

    TheaterTypeAddModifyForm *modifyForm = new TheaterTypeAddModifyForm(this, tt);
    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();

}

// Executes when is_pbtOK_clicked signal from TheaterTypeAddModifyForm is emitted.
// @param bool clicked. True if push button "pbtOK" was clicked.
void TheaterTypesForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}


