/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays information about movie's cast. Allows look up, add new or delete cast members from movies.

*/

#include "moviecastform.h"
#include "ui_moviecastform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MovieCastForm::MovieCastForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MovieCastForm)
{
    ui->setupUi(this);
    // Prevent user from resizing window.
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledTitle->setPlaceholderText("Any");
    ui->ledDurationTo->setPlaceholderText("Any");
    ui->ledDurationFrom->setPlaceholderText("Any");

    ui->dedReleaseDateFrom->setDate(QDate::currentDate());
    ui->dedReleaseDateTo->setDate(QDate::currentDate());
    ui->dedShowingStartDate->setDate(QDate::currentDate());
    ui->dedShowingEndDate->setDate(QDate::currentDate());

    ui->ledFirstName->setPlaceholderText("Any");
    ui->ledMiddleName->setPlaceholderText("Any");
    ui->ledLastName->setPlaceholderText("Any");

    ui->dedDOBTo->setDate(QDate::currentDate());

    QString message = addItemsToWidgets();
    if(message != "Success")
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", message);
        close();
    }

    // Makes items from lvwGenres checkable.
    QListWidgetItem* item = 0;
    for(int i = 0; i < ui->lvwGenres->count(); ++i){
        item = ui->lvwGenres->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    // Executes slot when item from lvwGenres state has changed
    QObject::connect(ui->lvwGenres, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));

    ui->lvwGenres->item(0)->setCheckState(Qt::Checked);
    ui->lvwGenres->item(0)->setBackgroundColor(QColor("#ffffb2"));
}

MovieCastForm::~MovieCastForm()
{
    delete ui->tvwTable->model();
    if(model != 0)
        delete model;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Add Items to widgets that are able to contain items in form.
// @return QString. Value "Success" if items were correctly added to widgets.
QString MovieCastForm::addItemsToWidgets()
{
    QStringList ageRestrictionList = { "Any", "G", "PG", "PG-13", "R", "NC-17" };
    ui->cbxAgeRestriction->addItems(ageRestrictionList);

    QStringList genderList = { "Any", "Male", "Female" };
    ui->cbxGender->addItems(genderList);

    if(!conn.connOpen())
        return "Couldn't load country list!\n Error: " + conn.nativeErrorCode();

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM country;");

    countryList.append("Any");

    if(!query.exec())
        return "Couldn't load country list!\nError: " + query.lastError().nativeErrorCode();

    while(query.next())
    {
        countryIDList.append(query.value(0).toInt());
        countryList.append(query.value(1).toString());
    }

    query.prepare("SELECT * "
                  "FROM movie_genre;");

    genresList.append("Any");

    if(!query.exec())
        return "Couldn't load movie genres!\nError: " + query.lastError().nativeErrorCode();

    while(query.next())
    {
        genresIDList.append(query.value(0).toInt());
        genresList.append(query.value(1).toString());
    }

    query.prepare("SELECT * "
                  "FROM cast_position;");

    positionList.append("Any");

    if(!query.exec())
        return "Couldn't load cast positions!\nError: " + query.lastError().nativeErrorCode();

    while(query.next())
    {
        positionIDList.append(query.value(0).toInt());
        positionList.append(query.value(1).toString());
    }

    conn.connClose();

    ui->cbxMovieCountry->addItems(countryList);
    ui->cbxCastCountry->addItems(countryList);
    ui->lvwGenres->addItems(genresList);
    ui->cbxPosition->addItems(positionList);

    return "Success";
}

// Load table with information about cast from movies.
// @return bool. True if table was successfully loaded.
bool MovieCastForm::loadTable()
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

    queryText = "SELECT m.movie_id AS 'Movie ID', m.title AS Title, "
                "cm.cast_member_id AS 'Cast Member ID', cm.first_name AS 'First Name', "
                "cm.middle_name AS 'Middle Name', cm.last_name AS 'Last Name', cm.date_of_birth AS 'Date of Birth', c_c.name AS 'Country (Cast Member)', "
                "cm.gender AS Gender, cp.position AS Position, "
                "m.duration AS Duration, m.release_date AS 'Release Date', "
                "m.age_restriction AS 'Age Restriction', m.synopsis AS Synopsis, m.trailer AS Trailer, "
                "m.poster AS Poster, m.showing_start_date AS 'Showing Start Date', m.showing_end_date AS 'Showing End Date', "
                "c_m.name AS 'Country (Movie)', mg.genre AS Genre, "
                "m.country_id, mg.movie_genre_id, cm.country_id, mc.cast_member__cast_position_id "
                "FROM movie__cast_member mc "
                "INNER JOIN cast_member__cast_position mp "
                "ON mc.cast_member__cast_position_id = mp.id "
                "INNER JOIN cast_position cp "
                "ON mp.cast_position_id = cp.cast_position_id "

                "INNER JOIN cast_member cm "
                "ON mp.cast_member_id = cm.cast_member_id "
                "INNER JOIN country c_c "
                "ON cm.country_id = c_c.country_id "

                "RIGHT JOIN movie m "
                "ON m.movie_id = mc.movie_id "
                "INNER JOIN country c_m "
                "ON m.country_id = c_m.country_id "
                "INNER JOIN movie__movie_genre mm "
                "ON m.movie_id = mm.movie_id "
                "INNER JOIN movie_genre mg "
                "ON mm.movie_genre_id = mg.movie_genre_id "
                "WHERE 1 = 1 ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Add filters to query.

    QString title = "%" + ui->ledTitle->text() + "%";
    queryText += title != "%%"? " AND m.title LIKE :title" : "";

    int durationFrom = ui->ledDurationFrom->text().toInt();
    queryText += durationFrom != 0? " AND m.duration >= :durationFrom" : "";

    int durationTo = ui->ledDurationTo->text().toInt();
    queryText += durationTo != 0? " AND m.duration <= :durationTo" : "";

    QString releaseDateFrom = ui->dedReleaseDateFrom->date().toString("yyyy-M-d");
    queryText += ui->chkReleaseDateFrom->isChecked()? " AND m.release_date >= :releaseDateFrom" : "";

    QString releaseDateTo = ui->dedReleaseDateTo->date().toString("yyyy-M-d");
    queryText += ui->chkReleaseDateTo->isChecked()? " AND m.release_date <= :releaseDateTo" : "";

    QString showingStartDate = ui->dedShowingStartDate->date().toString("yyyy-M-d");
    queryText += ui->chkShowingStartDate->isChecked()? " AND m.showing_start_date >= :showingStartDate" : "";

    QString showingEndDate = ui->dedShowingEndDate->date().toString("yyyy-M-d");
    queryText += ui->chkShowingEndDate->isChecked()? " AND m.showing_end_date <= :showingEndDate" : "";

    QString ageRestriction = ui->cbxAgeRestriction->currentText();
    queryText += ageRestriction != "Any"? " AND m.age_restriction = :ageRestriction" : "";

    QString movieCountry = ui->cbxMovieCountry->currentText();
    queryText += movieCountry != "Any"? " AND c_m.name = :movieCountry" : "";

    QString firstName = ui->ledFirstName->text();
    queryText += firstName != ""? " AND cm.first_name = :firstName" : "";

    QString middleName = ui->ledMiddleName->text();
    queryText += middleName != ""? " AND cm.middle_name = :middleName" : "";

    QString lastName = ui->ledLastName->text();
    queryText += lastName != ""? " AND cm.last_name = :lastName" : "";

    QString dobFrom = ui->dedDOBFrom->date().toString("yyyy-M-d");
    queryText += ui->chkDOBFrom->isChecked()? " AND cm.date_of_birth >= :dobFrom" : "";

    QString dobTo = ui->dedDOBTo->date().toString("yyyy-M-d");
    queryText += ui->chkDOBTo->isChecked()? " AND cm.date_of_birth <= :dobTo" : "";

    QString castCountry = ui->cbxCastCountry->currentText();
    queryText += castCountry != "Any"? " AND c_c.name = :castCountry" : "";

    QString gender = "";
    switch (ui->cbxGender->currentIndex()) {
        case 1:
            gender = "M";
            queryText += " AND cm.gender = :gender";
            break;
        case 2:
            gender = "F";
            queryText +=" AND cm.gender = :gender";
    }

    QString position = ui->cbxPosition->currentText();
    queryText += position != "Any"? " AND cp.position = :position" : "";

    // List with checked genres from lvwGenres.
    QStringList genresList;
    if(!ui->lvwGenres->item(0)->checkState())
    {
        queryText += " AND mg.genre IN ( ";
        int genresListCursor = 0;
        for(int i = 1; i < ui->lvwGenres->count(); i++)
        {
            if(ui->lvwGenres->item(i)->checkState())
            {
                genresList.append(ui->lvwGenres->item(i)->text());
                queryText += ":genre" + QString::number(genresListCursor++) + ", ";
            }

        }

        queryText.replace(queryText.length()-2, 2, ") ");
        //queryText = queryText.left(queryText.lastIndexOf(QChar(','))) + ") ";

    }
    else
        genresList.append(0);

    query.prepare( queryText + " ORDER BY m.movie_id, cm.cast_member_id;");

    query.bindValue(":title", title);
    query.bindValue(":durationFrom", durationFrom);
    query.bindValue(":durationTo", durationTo);
    query.bindValue(":releaseDateFrom", releaseDateFrom);
    query.bindValue(":releaseDateTo", releaseDateTo);
    query.bindValue(":showingStartDate", showingStartDate);
    query.bindValue(":showingEndDate", showingEndDate);
    query.bindValue(":ageRestriction", ageRestriction);
    query.bindValue(":movieCountry", movieCountry);

    query.bindValue(":firstName", firstName);
    query.bindValue(":middleName", middleName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":dobFrom", dobFrom);
    query.bindValue(":dobTo", dobTo);
    query.bindValue(":castCountry", castCountry);
    query.bindValue(":gender", gender);
    query.bindValue(":position", position);

    // Bind genres to query text.
    if(genresList.at(0) != 0)
        for(int i = 0; i < genresList.count(); i++)
            query.bindValue(":genre" + QString::number(i), genresList.at(i));

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load movies!\nError: " + query.lastError().nativeErrorCode() + query.lastError().text());
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
    // Hides m.country_id, mg.movie_genre_id, cm.country_id, mc.cast_member__cast_position_id
    ui->tvwTable->hideColumn(23);
    ui->tvwTable->hideColumn(22);
    ui->tvwTable->hideColumn(21);
    ui->tvwTable->hideColumn(20);

    conn.connClose();
    return true;
}

// Reset fields to default value in form
void MovieCastForm::resetFields()
{
    ui->ledTitle->setText("");
    ui->ledDurationFrom->setText("");
    ui->ledDurationTo->setText("");

    ui->chkReleaseDateFrom->setCheckState(Qt::Unchecked);
    ui->chkReleaseDateTo->setCheckState(Qt::Unchecked);
    ui->chkShowingStartDate->setCheckState(Qt::Unchecked);
    ui->chkShowingEndDate->setCheckState(Qt::Unchecked);

    ui->cbxMovieCountry->setCurrentIndex(0);
    ui->cbxAgeRestriction->setCurrentIndex(0);

    ui->lvwGenres->item(0)->setBackgroundColor(QColor("#ffffb2"));
    ui->lvwGenres->item(0)->setCheckState(Qt::Checked);

    for(int i = 1; i < ui->lvwGenres->count(); i++)
    {
        ui->lvwGenres->item(i)->setBackgroundColor(QColor("#ffffff"));
        ui->lvwGenres->item(i)->setCheckState(Qt::Unchecked);
    }

    ui->ledFirstName->setText("");
    ui->ledMiddleName->setText("");
    ui->ledLastName->setText("");
    ui->cbxGender->setCurrentIndex(0);

    ui->dedDOBFrom->setDate(QDate::currentDate());

    ui->cbxPosition->setCurrentIndex(0);
    ui->cbxCastCountry->setCurrentIndex(0);

    ui->chkDOBTo->setChecked(false);
    ui->chkDOBFrom->setChecked(false);
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void MovieCastForm::on_chkReleaseDateFrom_stateChanged(int is_checked)
{
    ui->dedReleaseDateFrom->setEnabled(is_checked);
}

void MovieCastForm::on_chkReleaseDateTo_stateChanged(int is_checked)
{
    ui->dedReleaseDateTo->setEnabled(is_checked);
}

void MovieCastForm::on_chkShowingStartDate_stateChanged(int is_checked)
{
    ui->dedShowingStartDate->setEnabled(is_checked);
}

void MovieCastForm::on_chkShowingEndDate_stateChanged(int is_checked)
{
    ui->dedShowingEndDate->setEnabled(is_checked);
}

void MovieCastForm::on_pbtLoadTable_clicked()
{
    if(loadTable())
    {
        ui->pbtAddNew->setEnabled(true);
        ui->pbtDelete->setEnabled(true);
    }
    else
    {
        ui->pbtAddNew->setEnabled(false);
        ui->pbtDelete->setEnabled(false);
    }
}

void MovieCastForm::on_pbtClearFields_clicked()
{
    resetFields();
}

// Open MovieCastAddModifyForm to add cast members to the movie that was selected from the table.
void MovieCastForm::on_pbtAddNew_clicked()
{
    if(ui->tvwTable->selectionModel()->selectedRows().count() != 1)
    {
        QMessageBox::warning(this, "Error", "Please select a single row!");
        return;
    }

    QModelIndex index=ui->tvwTable->selectionModel()->currentIndex();

    Movie movie;
    movie.setId(index.sibling(index.row(),0).data().toInt());
    movie.setTitle(index.sibling(index.row(),1).data().toString());

    MovieCastAddModifyForm *addForm = new MovieCastAddModifyForm(this, countryList, genresList, movie);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
}

// Delete cast members that were selected from the table
void MovieCastForm::on_pbtDelete_clicked()
{
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of cast member(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get rows that were selected.
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // 2D List with Ids of movies and Ids of cast members that work in them
    QList<QList<int>> castList;

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   // Get first selected row data
    QModelIndex index = select.at(0);
    int row = index.row();
    castList.append({index.sibling(row,0).data().toInt(), index.sibling(row,23).data().toInt()});

    // Go thru selected rows and make sure there's no repeat movieId - cast_member__cast_position_id
    for(int i=1; i< select.count(); i++)
    {
      index = select.at(i);
      row = index.row();
      bool castFound = false;

      for(int j = 0; j < castList.length(); j++)
          if(castList[j][0] == index.sibling(row,0).data().toInt() && castList[j][1] == index.sibling(row,23).data().toInt())
          {
              castFound = true;
              break;
          }

      if(!castFound)
          castList.append({index.sibling(row,0).data().toInt(), index.sibling(row,23).data().toInt()});
    }

    // Says if there was an error while deleting cast members working in movies.
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete cast members(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete cast member(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete cast members working in movies.
   for(int i=0; i< castList.length(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM movie__cast_member "
                     "WHERE movie_id = :movieID AND cast_member__cast_position_id = :positionID;");
       query.bindValue(":movieID", castList[i][0]);
       query.bindValue(":positionID", castList[i][1]);

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't delete cast member(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       conn.commit();
       conn.connClose();
       QMessageBox::information(this, "Success", QString::number(castList.length()) + " Cast member(s) deleted!");
       loadTable();
   }
}

void MovieCastForm::on_chkDOBFrom_stateChanged(int is_checked)
{
    ui->dedDOBFrom->setEnabled(is_checked);
}

void MovieCastForm::on_chkDOBTo_stateChanged(int is_checked)
{
    ui->dedDOBTo->setEnabled(is_checked);
}

// Executes when state of an item in lvwGenres changes. Highlights checked items.
// @param QListWidgetItem *item. Item whose state was changed.
void MovieCastForm::highlightChecked(QListWidgetItem *item)
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

// Executes when push button "pbtOK" from MovieCastAddModifyForm was clicked.
// @param bool clicked. True if "pbtOK" was clicked. False if it wasn't
void MovieCastForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}
