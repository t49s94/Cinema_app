/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing movie.

*/

#include "moviesaddmodifyform.h"
#include "ui_moviesaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new movie.
MoviesAddModifyForm::MoviesAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList, QList<int> genreIDList,
                                         QStringList genreList) :
    QDialog(parent),
    ui(new Ui::MoviesAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(countryIDList, countryList, genreIDList);

    // Remove genre "Any"
    genreList.removeAt(0);

    // Add items to lwdGenres and make them checkable.
    for(int i = 0; i < genreList.length(); i++)
    {
        ui->lwdGenres->addItem(genreList.at(i));
        QListWidgetItem* item = ui->lwdGenres->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);

    }

    // Emit signal itemChanged when the state of an item from lwdGenres is changed. Execute highlightChecked which highlights
    // checked items.
    QObject::connect(ui->lwdGenres, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));

    ui->lblId->hide();
    ui->lblIdlabel->hide();

    this->setWindowTitle("New Movie");

    queryType = "insertion";
}

// Constructor for updating existing movie.
MoviesAddModifyForm::MoviesAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList,
                                         QList<int> genreIDList, QStringList genreList, Movie oldMovie):
    QDialog(parent),
    ui(new Ui::MoviesAddModifyForm)
{
    ui->setupUi(this);

    initializeForm(countryIDList, countryList, genreIDList);

    // Remove genre "Any"
    genreList.removeAt(0);

    this->oldMovie = oldMovie;

    // Add items to lwdGenres and make them checkable.
    for(int i = 0; i < genreList.length(); i++)
    {
        ui->lwdGenres->addItem(genreList.at(i));
        QListWidgetItem* item = ui->lwdGenres->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        // True means movie has genre so we check it.
        bool isGenreChecked = false;
        // Search for genre in oldMovie genreList.
        for(int j = 0; j < oldMovie.getGenreList().count(); j++)
        {
            if(oldMovie.getGenreAt(j).getGenre() == item->text())
            {
                item->setCheckState(Qt::Checked);
                oldCheckedGenreIdxList.append(i);
                isGenreChecked = true;
                break;
            }
        }

        // Is mandatory to set item state.
        if (!isGenreChecked)
            item->setCheckState(Qt::Unchecked);

    }

    // Emit signal itemChanged when the state of an item from lwdGenres is changed. Execute highlightChecked which highlights
    // checked items.
    QObject::connect(ui->lwdGenres, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItemIdx*)));

    ui->lblId->setText(QString::number(oldMovie.getId()));
    ui->ledTitle->setText(oldMovie.getTitle());
    ui->spbDuration->setValue(oldMovie.getDuration());
    ui->dedReleaseDate->setDate(oldMovie.getReleaseDate());
    ui->cbxAgeRestriction->setCurrentText(oldMovie.getAgeRestriction());
    ui->tdtSynopsis->setText(oldMovie.getSynopsis());
    ui->ledTrailer->setText(oldMovie.getTrailer());
    ui->ledPoster->setText(oldMovie.getPoster());
    ui->dedShowingStartDate->setDate(oldMovie.getShowingStartDate());
    ui->dedShowingEndDate->setDate(oldMovie.getShowingEndDate());
    ui->cbxCountry->setCurrentText(oldMovie.getCountry().getName());

    this->setWindowTitle("Update Movie");

    queryType = "update";
}

MoviesAddModifyForm::~MoviesAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initialize some features of Dialog.
// @param QList<int> countryIDList. List with Ids of countries.
// @param QStringList countryList.
// @param QList<int> genreIDList. List with Ids of genres.
void MoviesAddModifyForm::initializeForm(QList<int> countryIDList, QStringList countryList, QList<int> genreIDList)
{
    // Prevents user from resizing Dialog.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Remove country "Any"
    countryList.removeAt(0);
    ui->cbxCountry->addItems(countryList);

    QStringList ageRestrictionList = { "G", "PG", "PG-13", "R", "NC-17" };
    ui->cbxAgeRestriction->addItems(ageRestrictionList);

    this->countryIDList = countryIDList;
    this->genreIDList = genreIDList;
}

// Checks if required fields were filled out.
// @param QList<MovieGenre> genreList.
// @return bool. True if all required fields were filled out
bool MoviesAddModifyForm::areFieldsNotBlank(QList<MovieGenre> genreList)
{
    // Error message
    QString message = "Please fill following fields:";

    if(ui->ledTitle->text() == "")
        message += ", Title";
    if(ui->spbDuration->value() == 0)
        message += ", Duration";
    if(genreList.length() == 0)
        message += ", Genres";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    // Returns true if all the required fields were filled up
    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;
}

// Insert new Movie into DB.
// @param Movie newMovie. Movie that's gonna be inserted.
// @return bool. True if movie was successfully inserted.
bool MoviesAddModifyForm::addMovie(Movie newMovie)
{
    QSqlQuery query;

    query.prepare("INSERT into movie (title, duration, release_date, age_restriction, synopsis, trailer, "
                  "poster, showing_start_date, showing_end_date, country_id) "
                  "VALUES (:title, :duration, :release_date, :age_restriction, :synopsis, :trailer, "
                  ":poster, :showing_start_date, :showing_end_date, :country_id);");

    query.bindValue(":title", newMovie.getTitle());
    query.bindValue(":duration", newMovie.getDuration());
    query.bindValue(":release_date", newMovie.getReleaseDate());
    query.bindValue(":age_restriction", newMovie.getAgeRestriction());
    query.bindValue(":synopsis", newMovie.getSynopsis());
    query.bindValue(":trailer", newMovie.getTrailer());
    query.bindValue(":poster", newMovie.getPoster());
    query.bindValue(":showing_start_date", newMovie.getShowingStartDate());
    query.bindValue(":showing_end_date", newMovie.getShowingEndDate());
    query.bindValue(":country_id", newMovie.getCountry().getId());

    if(!query.exec())
    {
        conn.connClose();

        QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return false;
    }

    query.prepare("SELECT LAST_INSERT_ID();");
    query.exec();

    if(!query.next())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return false;
    }

    int insertedID = query.value(0).toInt();

    foreach(MovieGenre genre, newMovie.getGenreList())
    {
        query.prepare("INSERT INTO movie__movie_genre "
                      "VALUES (:insertedID, :movieGenreID);");
        query.bindValue(":insertedID", insertedID);
        query.bindValue(":movieGenreID", genre.getId());

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                                 " failed!\n Error: " + query.lastError().nativeErrorCode());
            return false;
        }
    }

    return true;
}

// Update existing movie in the DB.
// @param Movie newMovie. Movie with updated data.
// @return bool. True if movie was successfully updated.
bool MoviesAddModifyForm::updateMovie(Movie newMovie)
{
    QSqlQuery query;

    // Copy of oldCheckedGenreIdxList. We will be removing the genres that are found from this copy. The genres left in this copy are the genres
    // that the user removed from the movie.
    QList<int> auxOldChecked = oldCheckedGenreIdxList;
    // If user added or removed genres from movie
    if(oldCheckedGenreIdxList != newCheckedGenreIdxList)
    {
        for(int i = 0; i < newCheckedGenreIdxList.count(); i++)
        {
            bool genreFound = false;
            // Try to find genre from newCheckedGenreIdxList(i) in auxOldChecked(j). Remove genre if it was found. If genre was found means that this
            // genre wasn't removed from movie.
            for(int j = 0; j < auxOldChecked.count(); j++)
                if(newCheckedGenreIdxList.at(i) == auxOldChecked.at(j))
                {
                    genreFound = true;
                    auxOldChecked.removeAt(j);
                    break;
                }

            // If genre wasn't found, means that genre is a new genre that wasn't in the original list of genres from movie.
            // So we insert it into DB.
            if(!genreFound)
            {
                query.prepare("INSERT INTO movie__movie_genre "
                              "VALUES (:movieID, :movieGenreID);");
                query.bindValue(":movieID", newMovie.getId());
                query.bindValue(":movieGenreID", newMovie.getGenreAt(i).getId());

                if(!query.exec())
                {
                    conn.connClose();
                    QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                                         " failed!\n Error: " + query.lastError().nativeErrorCode());
                    return false;
                }
            }

        }

        // The genres left in auxOldChecked are the genres that the user removed from movie.
        for(int i = 0; i < auxOldChecked.count(); i++)
        {
            query.prepare("DELETE FROM movie__movie_genre "
                          "WHERE movie_id = :movieID AND movie_genre_id = :movieGenreID");
            query.bindValue(":movieID", oldMovie.getId());
            query.bindValue(":movieGenreID", genreIDList.at(auxOldChecked.at(i)));

            if(!query.exec())
            {
                conn.connClose();
                QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                                     " failed!\n Error: " + query.lastError().nativeErrorCode());
                return false;
            }
        }

    }

    QString queryText = "UPDATE movie ";
    // List with updated fields from movie.
    QStringList setsList;

    if(oldMovie.getTitle() != newMovie.getTitle())
        setsList.append(", title = :title ");
    if(oldMovie.getDuration() != newMovie.getDuration())
        setsList.append(", duration = :duration ");
    if(oldMovie.getReleaseDate() != newMovie.getReleaseDate())
        setsList.append(", release_date = :release_date ");
    if(oldMovie.getAgeRestriction() != newMovie.getAgeRestriction())
        setsList.append(", age_restriction = :age_restriction ");
    if(oldMovie.getSynopsis() != newMovie.getSynopsis())
        setsList.append(", synopsis = :synopsis ");
    if(oldMovie.getTrailer() != newMovie.getTrailer())
        setsList.append(", trailer = :trailer ");
    if(oldMovie.getPoster() != newMovie.getPoster())
        setsList.append(", poster = :poster ");
    if(oldMovie.getShowingStartDate() != newMovie.getShowingStartDate())
        setsList.append(", showing_start_date = :showing_start_date ");
    if(oldMovie.getShowingEndDate() != newMovie.getShowingEndDate())
        setsList.append(", showing_end_date = :showing_end_date ");
    if(oldMovie.getCountry().getId() != newMovie.getCountry().getId())
        setsList.append(", country_id = :country_id ");

    if(setsList.length() > 0)
    {
        // Append the updated fields to update query
        for(int i = 0; i < setsList.length(); i++)
            queryText += setsList.at(i);

        queryText += " WHERE movie_id = :id;";

        QString subStr(","); // String to replace.
        QString newStr("SET"); // Replacement string.
        queryText.replace(queryText.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

        query.prepare(queryText);

        query.bindValue(":id", newMovie.getId());
        query.bindValue(":title", newMovie.getTitle());
        query.bindValue(":duration", newMovie.getDuration());
        query.bindValue(":release_date", newMovie.getReleaseDate());
        query.bindValue(":age_restriction", newMovie.getAgeRestriction());
        query.bindValue(":synopsis", newMovie.getSynopsis());
        query.bindValue(":trailer", newMovie.getTrailer());
        query.bindValue(":poster", newMovie.getPoster());
        query.bindValue(":showing_start_date", newMovie.getShowingStartDate());
        query.bindValue(":showing_end_date", newMovie.getShowingEndDate());
        query.bindValue(":country_id", newMovie.getCountry().getId());

        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                                 " failed!\n Error: " + query.lastError().nativeErrorCode());
            return false;
        }

    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

// Executes when user checks / unchecks item from "lwdGenres". Highlights checked items.
// @param QListWidgetItem *item. Item that was checked / unchecked.
void MoviesAddModifyForm::highlightChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
        item->setBackgroundColor(QColor("#ffffb2"));
    else
        item->setBackgroundColor(QColor("#ffffff"));
}

// Open QFileDialog to select Trailer file.
void MoviesAddModifyForm::on_pbtTrailer_clicked()
{
    QFileDialog dialog;
    ui->ledTrailer->setText(dialog.getOpenFileName(this, "Open the trailer file",
                                                   "C://Users//Owner//Desktop//Mis documentos//Portafolio//Cinema mysql db//Trailer files",
                                                   "MP4 files (*.mp4);;AVI files (*.avi);;WMV files (*.wmv);;MOV files (*.mov);;FLV files(*.flv)"));

}

// Open QFileDialog to select Poster file.
void MoviesAddModifyForm::on_pbtPoster_clicked()
{
    QFileDialog dialog;
    ui->ledPoster->setText(dialog.getOpenFileName(this, "Open the poster file",
                                                   "C://Users//Owner//Desktop//Mis documentos//Portafolio//Cinema mysql db//Poster files",
                                                   "JPEG files (*.jpeg);;GIF files (*.gif);;PNG files (*.png);;TIFF files (*.tiff)"));
}

void MoviesAddModifyForm::on_pbtOk_clicked()
{
    QString action = queryType;
    action[0] = action[0].toUpper();
    // Asks if user wants to proceed. Return if not
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of movie?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    Movie newMovie;

    newMovie.setId(ui->lblId->text().toInt());
    newMovie.setTitle(ui->ledTitle->text());
    newMovie.setDuration(ui->spbDuration->value());
    newMovie.setReleaseDate(ui->dedReleaseDate->date());
    newMovie.setAgeRestriction(ui->cbxAgeRestriction->currentText());
    newMovie.setSynopsis(ui->tdtSynopsis->toPlainText());
    newMovie.setTrailer(ui->ledTrailer->text());
    newMovie.setPoster(ui->ledPoster->text());
    newMovie.setShowingStartDate(ui->dedShowingStartDate->date());
    newMovie.setShowingEndDate(ui->dedShowingEndDate->date());
    newMovie.setCountry(Country(countryIDList.at(ui->cbxCountry->currentIndex()), ui->cbxCountry->currentText()));

    QList<MovieGenre> genreList;
    for(int i = 0; i < ui->lwdGenres->count(); i++)
    {
        if(ui->lwdGenres->item(i)->checkState() == Qt::Checked)
        {
            genreList.append(MovieGenre(genreIDList.at(i), ui->lwdGenres->item(i)->text()));
            newCheckedGenreIdxList.append(i);
        }

    }

    // Check if required fields in form were filled out.
    if(!areFieldsNotBlank(genreList))
        return;

    newMovie.setGenreList(genreList);

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    if(!conn.transaction())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    if(queryType == "insertion")
    {
        if(!addMovie(newMovie))
            return;
    }
    else
    {
        if(!updateMovie(newMovie))
            return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Movie \"" + newMovie.getTitle() + "\" " + queryType +
                             " failed!\n Error: " + conn.nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Movie \"" + newMovie.getTitle() + "\" " + queryType + " successful!");

    // Means that push button "pbtOk" was clicked.
    emit is_pbtOK_clicked(true);
    close();
}

void MoviesAddModifyForm::on_pbtCancel_clicked()
{
    // Means that push button "pbtOk" wasn't clicked.
    emit is_pbtOK_clicked(false);
    close();
}
