/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays the theaters stored in DB. Allows look up, add new, delete or updating existing theater.

*/

#include "theatersform.h"
#include "ui_theatersform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

TheatersForm::TheatersForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TheatersForm)
{
    ui->setupUi(this);
    // Prevent user from resizing Window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledName->setPlaceholderText("Any");

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load data!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM theater_type;");

    theaterTypesList.append({"0", "Any"});
    ui->cbxTheaterType->addItem("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load theater types!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        theaterTypesList.append({query.value(0).toString(), query.value(1).toString()});
        ui->cbxTheaterType->addItem(query.value(1).toString());
    }

    conn.connClose();

}

TheatersForm::~TheatersForm()
{
    if(model != 0)
        delete model;
    delete ui->tvwTable->model();

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Loads table with information about theaters.
// @return bool. True if table was successfully loaded.
bool TheatersForm::loadTable()
{
    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load theaters!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT t.theater_id AS ID, t.name AS Name, tt.name AS 'Theater type', COUNT(seat_id) AS 'Seat amount', "
                "SUM(CASE WHEN s.handicap > 0 THEN 1 ELSE 0 END) AS 'Handicap seat amount', tt.theater_type_id "
                "FROM theater t "
                "INNER JOIN seat s "
                "ON t.theater_id = s.theater_id "
                "INNER JOIN theater_type tt "
                "ON t.theater_type_id = tt.theater_type_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Add filters to search query

    QString name = "%" + ui->ledName->text() + "%";
    queryText += name != "%%"? " AND t.name LIKE :name" : "";

    QString theaterType = ui->cbxTheaterType->currentText();
    queryText += theaterType != "Any"? " AND tt.name = :theater_type" : "";

    queryText += " GROUP BY t.theater_id HAVING 1 = 1 ";

    int minSeatAmount = ui->spbMinSeatAmount->value();
    queryText += minSeatAmount != 0? " AND COUNT(seat_id) >= :min_seat_amount" : "";

    int maxSeatAmount = ui->spbMaxSeatAmount->value();
    queryText += maxSeatAmount != 0? " AND COUNT(seat_id) <= :max_seat_amount" : "";

    int minHandicapSeatAmount = ui->spbMinHandicapSeatAmount->value();
    queryText += minHandicapSeatAmount != 0? " AND SUM(CASE WHEN s.handicap > 0 THEN 1 ELSE 0 END) >= :min_handicap_seat_amount" : "";

    int maxHandicapSeatAmount = ui->spbMaxHandicapSeatAmount->value();
    queryText += maxHandicapSeatAmount != 0? " AND SUM(CASE WHEN s.handicap > 0 THEN 1 ELSE 0 END) <= :max_handicap_seat_amount" : "";

    query.prepare( queryText + " ORDER BY t.theater_id;");

    query.bindValue(":name", name);
    query.bindValue(":theater_type", theaterType);
    query.bindValue(":min_seat_amount", minSeatAmount);
    query.bindValue(":max_seat_amount", maxSeatAmount);
    query.bindValue(":min_handicap_seat_amount", minHandicapSeatAmount);
    query.bindValue(":max_handicap_seat_amount", maxHandicapSeatAmount);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load theaters!\nError: " + query.lastError().nativeErrorCode());
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
        items[2]->setData(query.value (2).toString (), Qt::DisplayRole);
        items[3]->setData(query.value (3).toInt (), Qt::DisplayRole);
        items[4]->setData(query.value (4).toInt (), Qt::DisplayRole);
        items[5]->setData(query.value (5).toInt (), Qt::DisplayRole);

        for(int column = 0; column < query.record().count(); column++)
            model->setItem(row, column, items[column]);
        row++;
    }

    ui->tvwTable->setModel(model);

    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hides tt.theater_type_id
    ui->tvwTable->hideColumn(5);

    conn.connClose();
    return true;

}

// Reset form's fields to default state.
void TheatersForm::resetFields()
{
    ui->ledName->setText("");
    ui->cbxTheaterType->setCurrentIndex(0);
    ui->spbMinSeatAmount->setValue(0);
    ui->spbMaxSeatAmount->setValue(0);
    ui->spbMinHandicapSeatAmount->setValue(0);
    ui->spbMaxHandicapSeatAmount->setValue(0);
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void TheatersForm::on_pbtLoadTable_clicked()
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

void TheatersForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void TheatersForm::on_pbtAddNew_clicked()
{
    TheaterAddModifyForm *addForm = new TheaterAddModifyForm(this, theaterTypesList);

    connect (addForm, SIGNAL (is_pbtSave_clicked(bool)), this, SLOT(on_is_pbtSave_clicked(bool)));
    addForm->exec();
}

// Delete selected theaters from table
void TheatersForm::on_pbtDelete_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of theater(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // QVector with Ids of theaters to be deleted
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

   // Says if there was an error during deletion of theaters
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete theater(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't delete theater(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete theaters
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM theater "
                     "WHERE theater_id = :theater_id;");
       query.bindValue(":theater_id", ids[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete theater(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", "Theater(s) deleted!");
       loadTable();
   }
}

// Update selected theater from table
void TheatersForm::on_pbtModify_clicked()
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

    Theater t;
    t.setId(index.sibling(index.row(),0).data().toInt());
    t.setName(index.sibling(index.row(),1).data().toString());
    t.setTheaterType(
        TheaterType(index.sibling(index.row(),5).data().toInt(), index.sibling(index.row(),2).data().toString(),
                    0,0)
    );

    TheaterAddModifyForm *modifyForm = new TheaterAddModifyForm(this, theaterTypesList, t);

    connect (modifyForm, SIGNAL (is_pbtSave_clicked(bool)), this, SLOT(on_is_pbtSave_clicked(bool)));
    modifyForm->exec();

}

// Executed when is_pbtSave_clicked is emitted.
// @param bool clicked. True if push button "pbtSave" was clicked
void TheatersForm::on_is_pbtSave_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
