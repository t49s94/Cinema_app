/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays information about employee positions . Allows look up, add new, delete or update existing employee position.

*/

#include "employeetypesform.h"
#include "ui_employeetypesform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

EmployeeTypesForm::EmployeeTypesForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EmployeeTypesForm)
{
    ui->setupUi(this);
    model = 0;
    // Prevents user from resizing window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);
}

EmployeeTypesForm::~EmployeeTypesForm()
{
    delete ui->tvwTable->model();
    if (model != 0)
        delete model;
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with employee positions.
// @return bool. True if table was successfully loaded
bool EmployeeTypesForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employee positions!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT employee_position_id AS ID, name AS Position "
                  "FROM employee_position "
                  "ORDER BY employee_position_id;");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load employee positions!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

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

void EmployeeTypesForm::on_pbtLoadTable_clicked()
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

// Delete selected records from table
void EmployeeTypesForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of employee position(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected records.
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // QVector that contains the ids from employee positions that are gonna be deleted
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate QVector ids.
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there was an error during the deletion of employee positions
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete employee position(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete employee position(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete employee positions
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;
       query.prepare("DELETE FROM employee_position "
                     "WHERE employee_position_id = :id;");
       query.bindValue(":id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete employee position(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }
   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete employee position(s)!\nError: " + conn.nativeErrorCode());
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Employee position(s) deleted!");
       loadTable();

   }

}

void EmployeeTypesForm::on_pbtAddNew_clicked()
{
    EmployeePositionsAddModifyForm *addForm = new EmployeePositionsAddModifyForm(this);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

// Update selected record
void EmployeeTypesForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    EmployeePosition ep;
    ep.setId(index.sibling(index.row(),0).data().toInt());
    ep.setName(index.sibling(index.row(),1).data().toString());

    EmployeePositionsAddModifyForm *modifyForm = new EmployeePositionsAddModifyForm(this, ep);
    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
    delete modifyForm;
}

// Executes when push button pbtOK from EmployeePositionsAddModifyForm is clicked
// @param bool clicked. True if clicked. False if it wasn't.
void EmployeeTypesForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

