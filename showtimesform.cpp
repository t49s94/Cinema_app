/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays information about showtimes stored in DB. Allows look up, add new, delete or update existing showtime.

*/

#include "showtimesform.h"
#include "ui_showtimesform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

ShowtimesForm::ShowtimesForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ShowtimesForm)
{
    ui->setupUi(this);

    // Prevents user from resizing Window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledMovie->setPlaceholderText("Any");

    ui->dedShowDateTo->setDate(QDate::currentDate());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load theaters!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM theater;");

    theatersList.append({"0", "Any"});
    ui->cbxTheater->addItem("Any");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load theaters!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        theatersList.append({ query.value(0).toString(), query.value(1).toString() });
        ui->cbxTheater->addItem(query.value(1).toString());
    }

    conn.connClose();
}

ShowtimesForm::~ShowtimesForm()
{
    if(model != 0)
        delete model;
    delete ui->tvwTable->model();

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Loads table with information about showtimes.
// @return bool. True if table was successfully loaded.
bool ShowtimesForm::loadTable()
{
    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load showtimes!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT s.showtime_id AS ID, m.title AS Movie, s.start_at AS 'Start at', s.end_at AS 'End at', "
                "s.show_date AS 'Show date', t.name AS 'Theater', m.movie_id, t.theater_id "
                "FROM showtime s "
                "INNER JOIN movie m "
                "ON s.movie_id = m.movie_id "
                "INNER JOIN theater t "
                "ON s.theater_id = t.theater_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Add filters to query

    QString title = "%" + ui->ledMovie->text() + "%";
    queryText += title != "%%"? " AND m.title LIKE :title" : "";

    QString startAt = ui->tmeStartAt->time().toString("hh:mm:ss");
    queryText += ui->chkStartAt->isChecked()? " AND s.start_at >= :start_at" : "";

    QString endAt = ui->tmeEndAt->time().toString("hh:mm:ss");
    queryText += ui->chkEndAt->isChecked()? " AND s.end_at <= :end_at" : "";

    QString showDateFrom = ui->dedShowDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkShowDateFrom->isChecked()? " AND s.show_date >= :show_date_from" : "";

    QString showDateTo = ui->dedShowDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkShowDateTo->isChecked()? " AND s.show_date <= :show_date_to" : "";

    QString theater = ui->cbxTheater->currentText();
    queryText += theater != "Any"? " AND t.name = :name" : "";

    query.prepare( queryText + " ORDER BY s.showtime_id;");

    query.bindValue(":title", title);
    query.bindValue(":start_at", startAt);
    query.bindValue(":end_at", endAt);
    query.bindValue(":show_date_from", showDateFrom);
    query.bindValue(":show_date_to", showDateTo);
    query.bindValue(":name", theater);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load showtimes!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    QStringList tableHeaders;
    for(int i = 0; i < query.record().count(); i++)
        tableHeaders.append(query.record().fieldName(i));

    model = new QStandardItemModel(query.size(), query.record().count());
    model->setHorizontalHeaderLabels(tableHeaders);

    int row =0;
    while(query.next())
    {
        QList<QStandardItem*> items;
        for(int i = 0; i < query.record().count(); i++)
            items.append(new QStandardItem());

        items[0]->setData(query.value (0).toInt (), Qt::DisplayRole);
        items[1]->setData(query.value (1).toString (), Qt::DisplayRole);
        items[2]->setData(query.value (2).toTime(), Qt::DisplayRole);
        items[3]->setData(query.value (3).toTime (), Qt::DisplayRole);
        items[4]->setData(query.value (4).toDate(), Qt::DisplayRole);
        items[5]->setData(query.value (5).toString(), Qt::DisplayRole);
        items[6]->setData(query.value (6).toInt (), Qt::DisplayRole);
        items[7]->setData(query.value (7).toInt (), Qt::DisplayRole);

        for(int column = 0; column < query.record().count(); column++)
            model->setItem(row, column, items[column]);

        row++;
    }

    ui->tvwTable->setModel(model);

    // Model that filters data in table
    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hides m.movie_id, t.theater_id
    ui->tvwTable->hideColumn(6);
    ui->tvwTable->hideColumn(7);

    conn.connClose();
    return true;
}

// Reset fields to their default state.
void ShowtimesForm::resetFields()
{
    ui->ledMovie->setText("");
    ui->tmeStartAt->setTime(QTime(12, 0, 0, 0));
    ui->tmeEndAt->setTime(QTime::currentTime());
    ui->dedShowDateFrom->setDate(QDate(2000, 1, 1));
    ui->dedShowDateTo->setDate(QDate::currentDate());
    ui->cbxTheater->setCurrentIndex(0);

    ui->chkStartAt->setChecked(Qt::Unchecked);
    ui->chkEndAt->setChecked(Qt::Unchecked);
    ui->chkShowDateFrom->setChecked(Qt::Unchecked);
    ui->chkShowDateTo->setChecked(Qt::Unchecked);
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void ShowtimesForm::on_pbtLoadTable_clicked()
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

void ShowtimesForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void ShowtimesForm::on_pbtAddNew_clicked()
{
    ShowtimeAddModifyForm *addForm = new ShowtimeAddModifyForm(this, theatersList);

    connect (addForm, SIGNAL (is_pbtSave_clicked(bool)), this, SLOT(on_is_pbtSave_clicked(bool)));
    addForm->exec();
}

// Deletes selected showtimes from table
void ShowtimesForm::on_pbtDelete_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of showtime(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete showtime(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't delete showtime(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   QSqlQuery query;

   // Deletes showtimes
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();

       query.prepare("DELETE FROM showtime "
                     "WHERE showtime_id = :showtime_id;");
       query.bindValue(":showtime_id", index.sibling(row,0).data().toInt());

       if(!query.exec())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't delete showtime(s)!\nError: " + query.lastError().nativeErrorCode());
           return;
       }
   }

   if(!conn.commit())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't delete showtime(s)!\nError: " + query.lastError().nativeErrorCode());
       return;
   }

   conn.connClose();
   QMessageBox::information(this, "Success", "Showtime(s) deleted!");
   loadTable();
}

// Updates selected showtime
void ShowtimesForm::on_pbtModify_clicked()
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

    Showtime s;
    s.setId(index.sibling(index.row(),0).data().toInt());
    s.setMovie(Movie(index.sibling(index.row(),6).data().toInt(), index.sibling(index.row(),1).data().toString(),
                     0, QDate(), "", "", "", "", QDate(), QDate(), Country(), QList<MovieGenre>()));
    s.setStartAt(index.sibling(index.row(),2).data().toTime());
    s.setEndAt(index.sibling(index.row(),3).data().toTime());
    s.setShowDate(index.sibling(index.row(),4).data().toDate());
    s.setTheater(Theater(index.sibling(index.row(),7).data().toInt(), index.sibling(index.row(),5).data().toString(),
                         TheaterType()));

    ShowtimeAddModifyForm *modifyForm = new ShowtimeAddModifyForm(this, theatersList, s);

    connect (modifyForm, SIGNAL (is_pbtSave_clicked(bool)), this, SLOT(on_is_pbtSave_clicked(bool)));
    modifyForm->exec();

}

// Executes when is_pbtSave_clicked signal is emitted.
// @param bool clicked. True if push button pbtSave was clicked.
void ShowtimesForm::on_is_pbtSave_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

void ShowtimesForm::on_chkStartAt_stateChanged(int checked)
{
    ui->tmeStartAt->setEnabled(checked);
}

void ShowtimesForm::on_chkEndAt_stateChanged(int checked)
{
    ui->tmeEndAt->setEnabled(checked);
}

void ShowtimesForm::on_chkShowDateFrom_stateChanged(int checked)
{
    ui->dedShowDateFrom->setEnabled(checked);
}

void ShowtimesForm::on_chkShowDateTo_stateChanged(int checked)
{
    ui->dedShowDateTo->setEnabled(checked);
}
