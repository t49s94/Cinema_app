#include "castpositionsform.h"
#include "ui_castpositionsform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window displays cast positions stored in DB.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

CastPositionsForm::CastPositionsForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CastPositionsForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;
}

CastPositionsForm::~CastPositionsForm()
{
    delete ui->tvwTable->model();
    if (model != 0)
        delete model;

    delete ui;
}

//------------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with information about cast positions
bool CastPositionsForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM cast_position "
                  "ORDER BY cast_position_id;");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

    // Model that will allow filter in the table
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);
    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);

    conn.connClose();
    return true;
}

//------------------------------------------------------------------------------------------------------------------------------
// Slots

void CastPositionsForm::on_pbtLoadTable_clicked()
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

void CastPositionsForm::on_pbtDelete_clicked()
{
    // Ask if user wants to proceed. Returns if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of cast position(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Vector of IDs of cast positions that will be deleted
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate Vector ids.
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Value is true if error occured during the deletion of cast positions
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete cast position(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete cast position(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete cast positions
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;
       query.prepare("DELETE FROM cast_position "
                     "WHERE cast_position_id = :id;");
       query.bindValue(":id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete cast position(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }
   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete cast position(s)!\nError: " + conn.nativeErrorCode());
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Cast position(s) deleted!");
       loadTable();
   }

}

void CastPositionsForm::on_pbtAddNew_clicked()
{
    CastPositionsAddModifyForm *addForm = new CastPositionsAddModifyForm(this);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

void CastPositionsForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() == 1)
    {
        QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

        CastPosition cp(index.sibling(index.row(),0).data().toInt(), index.sibling(index.row(),1).data().toString());

        CastPositionsAddModifyForm *modifyForm = new CastPositionsAddModifyForm(this, cp);
        connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
        modifyForm->exec();
        delete modifyForm;
    }
    else
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
    }
}

// Executes when push button "pbtOK" from CastPositionsAddModifyForm is clicked
void CastPositionsForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
