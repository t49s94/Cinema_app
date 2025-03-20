/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays movies stored in DB. Allows look up, add new, delete or update existing movie.

*/

#include "moviesform.h"
#include "ui_moviesform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MoviesForm::MoviesForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MoviesForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledTitle->setPlaceholderText("Any");

    QStringList ageRestrictionList = { "Any", "G", "PG", "PG-13", "R", "NC-17" };
    ui->cbxAgeRestriction->addItems(ageRestrictionList);

    ui->dedReleaseDateFrom->setDate(QDate::currentDate());
    ui->dedReleaseDateTo->setDate(QDate::currentDate());
    ui->dedShowingStartDate->setDate(QDate::currentDate());
    ui->dedShowingEndDate->setDate(QDate::currentDate());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load country list!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM country;");

    countryList.append("Any");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load country list!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        countryIDList.append(query.value(0).toInt());
        countryList.append(query.value(1).toString());
    }

    query.prepare("SELECT * "
                  "FROM movie_genre;");

    genresList.append("Any");

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load country list!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        genresIDList.append(query.value(0).toInt());
        genresList.append(query.value(1).toString());
    }

    conn.connClose();

    ui->cbxCountry->addItems(countryList);
    ui->lvwGenres->addItems(genresList);

    QListWidgetItem* item = 0;
    // Makes items from lvwGenres checkable.
    for(int i = 0; i < ui->lvwGenres->count(); ++i){
        item = ui->lvwGenres->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    // Connect signal itemChanged from lvwGenres to form and executes Slot highlightChecked when state from item in lvwGenres changes.
    // It highlights checked items.
    QObject::connect(ui->lvwGenres, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));

    ui->lvwGenres->item(0)->setCheckState(Qt::Checked);
    ui->lvwGenres->item(0)->setBackgroundColor(QColor("#ffffb2"));
}

MoviesForm::~MoviesForm()
{
    delete ui->tvwTable->model();
    if(model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with information about movies.
// @return bool. True if table was successfully loaded.
bool MoviesForm::loadTable()
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
    QString queryText;

    queryText = "SELECT m.movie_id AS ID, m.title AS Title, m.duration AS Duration, m.release_date AS 'Release date', "
                "m.age_restriction AS 'Age restriction', m.synopsis AS Synopsis, m.trailer AS Trailer, m.poster AS Poster, "
                "m.showing_start_date AS 'Showing Start Date', m.showing_end_date AS 'Showing End Date', c.name AS Country, mg.genre AS Genre, "
                "c.country_id, mg.movie_genre_id "
                "FROM movie_genre mg "
                "INNER JOIN movie__movie_genre mm "
                "ON mg.movie_genre_id = mm.movie_genre_id "
                "INNER JOIN movie m "
                "ON mm.movie_id = m.movie_id "
                "INNER JOIN country c "
                "ON m.country_id = c.country_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Append filters to query text.

    QString title = "%" + ui->ledTitle->text() + "%";
    queryText += title != "%%"? " AND title LIKE :title" : "";

    int durationFrom = ui->spbDurationFrom->value();
    queryText += durationFrom != 0? " AND duration >= :durationFrom" : "";

    int durationTo = ui->spbDurationTo->value();
    queryText += durationTo != 0? " AND duration <= :durationTo" : "";

    QString releaseDateFrom = ui->dedReleaseDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkReleaseDateFrom->isChecked()? " AND release_date >= :releaseDateFrom" : "";

    QString releaseDateTo = ui->dedReleaseDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkReleaseDateTo->isChecked()? " AND release_date <= :releaseDateTo" : "";

    QString showingStartDate = ui->dedShowingStartDate->date().toString("yyyy-M-d");
    queryText += ui->chkShowingStartDate->isChecked()? " AND showing_start_date >= :showingStartDate" : "";

    QString showingEndDate = ui->dedShowingEndDate->date().toString("yyyy-M-d");
    queryText += ui->chkShowingEndDate->isChecked()? " AND showing_end_date <= :showingEndDate" : "";

    QString ageRestriction = ui->cbxAgeRestriction->currentText();
    queryText += ageRestriction != "Any"? " AND age_restriction = :ageRestriction" : "";

    QString country = ui->cbxCountry->currentText();
    queryText += country != "Any"? " AND name = :country" : "";

    QStringList genresList;
    // Append genres that were checked
    if(!ui->lvwGenres->item(0)->checkState())
    {
        queryText += " AND genre IN ( ";
        int genresListCursor = 0;
        for(int i = 1; i < ui->lvwGenres->count(); i++)
        {
            if(ui->lvwGenres->item(i)->checkState())
            {
                genresList.append(ui->lvwGenres->item(i)->text());
                queryText += ":genre" + QString::number(genresListCursor++) + ", ";
            }

        }

        // Replace last ", " with ") "
        queryText.replace(queryText.length()-2, 2, ") ");
        //queryText = queryText.left(queryText.lastIndexOf(QChar(','))) + ") ";

    }
    else
        genresList.append(0);

    query.prepare( queryText + " ORDER BY m.movie_id;");

    query.bindValue(":title", title);
    query.bindValue(":durationFrom", durationFrom);
    query.bindValue(":durationTo", durationTo);
    query.bindValue(":releaseDateFrom", releaseDateFrom);
    query.bindValue(":releaseDateTo", releaseDateTo);
    query.bindValue(":showingStartDate", showingStartDate);
    query.bindValue(":showingEndDate", showingEndDate);
    query.bindValue(":ageRestriction", ageRestriction);
    query.bindValue(":country", country);

    if(genresList.at(0) != 0)
        for(int i = 0; i < genresList.count(); i++)
            query.bindValue(":genre" + QString::number(i), genresList.at(i));


    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load movies!\nError: " + query.lastError().nativeErrorCode());
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
    // Hides c.country_id, mg.movie_genre_id.
    ui->tvwTable->hideColumn(13);
    ui->tvwTable->hideColumn(12);

    conn.connClose();
    return true;
}

// Reset fields to default value in form.
void MoviesForm::resetFields()
{
    ui->ledTitle->setText("");
    ui->spbDurationFrom->setValue(0);
    ui->spbDurationTo->setValue(0);

    ui->chkReleaseDateFrom->setCheckState(Qt::Unchecked);
    ui->chkReleaseDateTo->setCheckState(Qt::Unchecked);
    ui->chkShowingStartDate->setCheckState(Qt::Unchecked);
    ui->chkShowingEndDate->setCheckState(Qt::Unchecked);

    ui->cbxCountry->setCurrentIndex(0);
    ui->cbxAgeRestriction->setCurrentIndex(0);

    ui->lvwGenres->item(0)->setBackgroundColor(QColor("#ffffb2"));
    ui->lvwGenres->item(0)->setCheckState(Qt::Checked);

    for(int i = 1; i < ui->lvwGenres->count(); i++)
    {
        ui->lvwGenres->item(i)->setBackgroundColor(QColor("#ffffff"));
        ui->lvwGenres->item(i)->setCheckState(Qt::Unchecked);
    }
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

// Executes when item in lvwGenres is checked / unchecked. Highlights checked items.
// @param QListWidgetItem *item. Item that was checked / unchecked.
void MoviesForm::highlightChecked(QListWidgetItem *item)
{
    // Temporary disconnects because since we change state of item inside this slot, signal itemChanged will be emitted and slot will be
    // called again and again creating an infinite loop.
    QObject::disconnect(ui->lvwGenres, SIGNAL(itemChanged(QListWidgetItem*)),
                        this, SLOT(highlightChecked(QListWidgetItem*)));

    // When item "Any" is checked, all the other items will be unchecked leaving item "Any" as the only one checked.
    if(item->text() == "Any")
    {
        item->setBackgroundColor(QColor("#ffffb2"));

        for(int i = 1; i < ui->lvwGenres->count(); i++)
        {
            ui->lvwGenres->item(i)->setBackgroundColor(QColor("#ffffff"));
            ui->lvwGenres->item(i)->setCheckState(Qt::Unchecked);
        }

    }
    // Make sure that item "Any" is unchecked and check the item from parameter.
    else
    {
        ui->lvwGenres->item(0)->setBackgroundColor(QColor("#ffffff"));
        ui->lvwGenres->item(0)->setCheckState(Qt::Unchecked);

        if(item->checkState() == Qt::Checked)
            item->setBackgroundColor(QColor("#ffffb2"));
        else
            item->setBackgroundColor(QColor("#ffffff"));

    }

    // Reconnect signal - slot.
    QObject::connect(ui->lvwGenres, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));
}

void MoviesForm::on_chkReleaseDateFrom_stateChanged(int is_checked)
{
    ui->dedReleaseDateFrom->setEnabled(is_checked);
}

void MoviesForm::on_chkReleaseDateTo_stateChanged(int is_checked)
{
    ui->dedReleaseDateTo->setEnabled(is_checked);
}

void MoviesForm::on_chkShowingStartDate_stateChanged(int is_checked)
{
    ui->dedShowingStartDate->setEnabled(is_checked);
}

void MoviesForm::on_chkShowingEndDate_stateChanged(int is_checked)
{
    ui->dedShowingEndDate->setEnabled(is_checked);
}

void MoviesForm::on_pbtLoadTable_clicked()
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

void MoviesForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void MoviesForm::on_pbtAddNew_clicked()
{
    MoviesAddModifyForm *addForm = new MoviesAddModifyForm(this, countryIDList, countryList, genresIDList, genresList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Deletes selected movies in table.
void MoviesForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of movie(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get selected rows from table.
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Vector that contains Ids of movies that are gonna be deleted.
    QVector<int> movieIds(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Populate QVector<int> movieIds
   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       movieIds[i] = index.sibling(row,0).data().toInt();
   }

   // Says if there was an error during the deletion of movies.
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete movie(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't delete movie(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete movies
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM movie "
                     "WHERE movie_id = :movieID;");
       query.bindValue(":movieID", movieIds[i]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete cast member(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(error)
   {
       QMessageBox::warning(this, "Error", "Couldn't load cast positions!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.commit())
   {
       conn.connClose();
       QMessageBox::warning(this, "Error", "Couldn't load cast positions!\nError: " + conn.nativeErrorCode());
       return;
   }

   conn.connClose();
   QMessageBox::information(this, "Success", "Cast member(s) deleted!");
   loadTable();

}

// Update selected movie from table
void MoviesForm::on_pbtModify_clicked()
{
    // Makes sure user only selected a single row.
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

    Movie movie;

    movie.setId(index.sibling(index.row(),0).data().toInt());
    movie.setTitle(index.sibling(index.row(),1).data().toString());
    movie.setDuration(index.sibling(index.row(),2).data().toInt());
    movie.setReleaseDate(index.sibling(index.row(),3).data().toString());
    movie.setAgeRestriction(index.sibling(index.row(),4).data().toString());
    movie.setSynopsis(index.sibling(index.row(),5).data().toString());
    movie.setTrailer(index.sibling(index.row(),6).data().toString());
    movie.setPoster(index.sibling(index.row(),7).data().toString());
    movie.setShowingStartDate(index.sibling(index.row(),8).data().toString());
    movie.setShowingEndDate(index.sibling(index.row(),9).data().toString());
    movie.setCountry(Country(index.sibling(index.row(),12).data().toInt(), index.sibling(index.row(),10).data().toString()));

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't open Movie form!\nError: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    query.prepare("SELECT mg.movie_genre_id, mg.genre "
                  "FROM movie_genre mg "
                  "WHERE mg.movie_genre_id IN ( "
                  "SELECT mm.movie_genre_id "
                  "FROM movie__movie_genre mm "
                  "WHERE mm.movie_id = :id);");

    query.bindValue(":id", movie.getId());

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't open Movie form!\nError: " + query.lastError().nativeErrorCode());
        return;
    }

    while(query.next())
        movie.appendGenre(MovieGenre(query.value(0).toInt(), query.value(1).toString()));

    MoviesAddModifyForm *modifyForm = new MoviesAddModifyForm(this, countryIDList, countryList, genresIDList, genresList, movie);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
}

// Executes when is_pbtOK_clicked signal is emitted. Says if push button "pbtOk" from MoviesAddModifyForm was clicked.
// @param bool clicked. True if pbtOk was clicked. False if it wasn't.
void MoviesForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
