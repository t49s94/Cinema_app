/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays movie genres. Allows look up, add new, delete or update existing movie genre.

*/

#include "moviegenresform.h"
#include "ui_moviegenresform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MovieGenresForm::MovieGenresForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MovieGenresForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;
}

MovieGenresForm::~MovieGenresForm()
{
    if (model != 0)
        delete model;
    delete ui->tvwTable->model();

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with movie genres.
// @return bool. True if table was successfully loaded.
bool MovieGenresForm::loadTable()
{
    delete ui->tvwTable->model();
    delete model;
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load movie genres!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;

    query.prepare("SELECT movie_genre_id AS ID, genre AS Genre "
                  "FROM movie_genre;");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load movie genres!\nError: " + query.lastError().nativeErrorCode());
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

void MovieGenresForm::on_pbtLoadTable_clicked()
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

void MovieGenresForm::on_pbtAddNew_clicked()
{
    MovieGenresAddModifyForm *addForm = new MovieGenresAddModifyForm(this);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Deletes selected rows
void MovieGenresForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of movie genre(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows.
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Vector with ids of movie genres that are gonna be deleted
    QVector<int> ids(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate  QVector<int> ids.
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       ids[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there was an error during the deletion of movie genres
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't load movie genres!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete movie genre(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete movie genres.
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;
       query.prepare("DELETE FROM movie_genre "
                     "WHERE movie_genre_id = :id;");
       query.bindValue(":id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete movie genre(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }
   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete movie genre(s)!\nError: " + conn.nativeErrorCode());
           return;
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Movie genre(s) deleted!");
       loadTable();
    }

}

// Update rows that were selected
void MovieGenresForm::on_pbtModify_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::warning(this, "Error", "You can only modify a row at the time!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    MovieGenre mg;
    mg.setId(index.sibling(index.row(),0).data().toInt());
    mg.setGenre(index.sibling(index.row(),1).data().toString());

    MovieGenresAddModifyForm *modifyForm = new MovieGenresAddModifyForm(this, mg);
    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
\
}

// Executes when push button "pbtOk" from MovieGenresAddModifyForm was clicked.
// @param bool clicked. True if pbtOk was clicked. False if it wasn't.
void MovieGenresForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
