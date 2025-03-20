/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user to add new or update existing showtime.

*/

#include "showtimeaddmodifyform.h"
#include "ui_showtimeaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new showtime
ShowtimeAddModifyForm::ShowtimeAddModifyForm(QWidget *parent, QList<QList<QString>> theatersList) :
    QDialog(parent),
    ui(new Ui::ShowtimeAddModifyForm)
{
    ui->setupUi(this);

    initialize(&theatersList);

    ui->dedShowDate->setMinimumDate(QDate::currentDate());

    ui->lblId->hide();
    ui->lbllblId->hide();
    this->setWindowTitle("New Showtime");

    queryType = "insertion";
}

// Constructor for updating existing showtime
ShowtimeAddModifyForm::ShowtimeAddModifyForm(QWidget *parent, QList<QList<QString>> theatersList, Showtime oldShowtime) :
    QDialog(parent),
    ui(new Ui::ShowtimeAddModifyForm)
{
    ui->setupUi(this);

    initialize(&theatersList);

    this->oldShowtime = oldShowtime;
    newShowtime.setMovie(oldShowtime.getMovie());

    ui->lblId->setText(QString::number(oldShowtime.getId()));
    ui->lblMovie->setText(oldShowtime.getMovie().getTitle());
    ui->tmeStartAt->setTime(oldShowtime.getStartAt());
    ui->tmeEndAt->setTime(oldShowtime.getEndAt());
    ui->dedShowDate->setDate(oldShowtime.getShowDate());
    ui->cbxTheater->setCurrentText(oldShowtime.getTheater().getName());

    this->setWindowTitle("Update Showtime");

    queryType = "update";
}

ShowtimeAddModifyForm::~ShowtimeAddModifyForm()
{
    delete model;
    delete ui->tvwTable->model();

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initialize a few features of form.
// @param QList<QList<QString>>* theatersList. 2D List with "ID" and "Theater name"
void ShowtimeAddModifyForm::initialize(QList<QList<QString>>* theatersList)
{
    // Prevent users from resizing Dialog
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Delete "Any" theater
    theatersList->removeAt(0);
    this->theatersList = *theatersList;

    for(int i = 0; i < theatersList->length(); i++)
        ui->cbxTheater->addItem(theatersList->at(i).at(1));

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load movies!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT m.movie_id AS ID, m.title AS Title, m.duration AS Duration, m.release_date AS 'Release date', "
                  "m.age_restriction AS 'Age restriction', m.synopsis AS Synopsis, m.trailer AS Trailer, m.poster AS Poster, "
                  "m.showing_start_date AS 'Showing start date', m.showing_end_date AS 'Showing end date', c.name AS Country, "
                  "c.country_id "
                  "FROM movie m "
                  "INNER JOIN country c "
                  "ON m.country_id = c.country_id "
                  "WHERE m.showing_start_date <= :current_date "
                  "AND m.showing_end_date >= :current_date "
                  "ORDER BY m.movie_id;");

    query.bindValue(":current_date", QDate::currentDate().toString("yyyy-MM-dd"));

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load movies!\nError: " + query.lastError().nativeErrorCode());
        close();
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
        items[2]->setData(query.value (2).toInt (), Qt::DisplayRole);
        items[3]->setData(query.value (3).toDate(), Qt::DisplayRole);

        items[4]->setData(query.value (4).toString (), Qt::DisplayRole);
        items[5]->setData(query.value (5).toString (), Qt::DisplayRole);
        items[6]->setData(query.value (6).toString (), Qt::DisplayRole);
        items[7]->setData(query.value (7).toString (), Qt::DisplayRole);

        items[8]->setData(query.value (8).toDate(), Qt::DisplayRole);
        items[9]->setData(query.value (9).toDate(), Qt::DisplayRole);
        items[10]->setData(query.value (10).toString(), Qt::DisplayRole);
        items[11]->setData(query.value (11).toInt(), Qt::DisplayRole);

        for(int column = 0; column < query.record().count(); column++)
            model->setItem(row, column, items[column]);

        row++;
    }

    ui->tvwTable->setModel(model);

    // Model that sorts data in table
    QSortFilterProxyModel *m = new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);

    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    // Hides c.country_id
    ui->tvwTable->hideColumn(11);

    ui->tvwTable->setSelectionBehavior(QAbstractItemView::SelectItems );
    ui->tvwTable->setSelectionMode(QAbstractItemView::SingleSelection );

    conn.connClose();
}

// Checks if required fields were filled out.
// @return bool. True if required fields were filled out.
bool ShowtimeAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->lblMovie->text() == "No movie selected...")
        message += ", Movie";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;
}

// Checks if fields have correct data
// @return bool. True if fields have correct data.
bool ShowtimeAddModifyForm::validateFields()
{
    // Turn min to sec
    int durationSecs = newShowtime.getMovie().getDuration() * 60;
    // Calculate the time when movie ends
    QTime showtimeEndTime = ui->tmeStartAt->time().addSecs(durationSecs);

    QString message = "";

    if(showtimeEndTime > ui->tmeEndAt->time())
    {
        QMessageBox::warning(this, "Error", "\"Showtime end at\" field can't be sooner than the time when movie actually ends!");
        return false;
    }

    return true;
}

// Creates query text for update.
// @param QString* queryText. QString that contains update query text
// @return bool. False if there were no changes made.
bool ShowtimeAddModifyForm::setUpdateQueryText(QString *queryText)
{
    *queryText = "UPDATE showtime ";
    QStringList setsList;

    if(oldShowtime.getMovie().getId() != newShowtime.getMovie().getId())
        setsList.append(", movie_id = :movie_id ");
    if(oldShowtime.getStartAt() != newShowtime.getStartAt())
        setsList.append(", start_at = :start_at ");
    if(oldShowtime.getEndAt() != newShowtime.getEndAt())
        setsList.append(", end_at = :end_at ");
    if(oldShowtime.getShowDate() != newShowtime.getShowDate())
        setsList.append(", show_date = :show_date ");
    if(oldShowtime.getTheater().getId() != newShowtime.getTheater().getId())
        setsList.append(", theater_id = :theater_id ");

    // Checks if at least one field was updated. Return if not
    if(setsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "No changes made!");
        return false;
    }

    for(int i = 0; i < setsList.length(); i++)
        *queryText += setsList.at(i);

    *queryText += " WHERE showtime_id = :showtime_id;";

    QString subStr(","); // String to replace.
    QString newStr("SET"); // Replacement string.
    queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void ShowtimeAddModifyForm::on_pbtOk_clicked()
{
    // Checks if required fields were filled out and have correct information
    if(!areFieldsNotBlank() || !validateFields())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of showtime?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newShowtime.setId(ui->lblId->text().toInt());
    newShowtime.setStartAt(ui->tmeStartAt->time());
    newShowtime.setEndAt(ui->tmeEndAt->time());
    newShowtime.setShowDate(ui->dedShowDate->date());
    newShowtime.setTheater(Theater(theatersList.at(ui->cbxTheater->currentIndex()).at(0).toInt(),
                                   theatersList.at(ui->cbxTheater->currentIndex()).at(1), TheaterType()));

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    if(!conn.transaction())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QString queryText = "";
    QSqlQuery query;

    if(queryType == "insertion")
        queryText = "INSERT INTO showtime (movie_id, start_at, end_at, show_date, theater_id) "
                    "VALUES (:movie_id, :start_at, :end_at, :show_date, :theater_id);";
    else
        if(!setUpdateQueryText(&queryText))
            return;

    query.prepare(queryText);

    query.bindValue(":showtime_id", newShowtime.getId());
    query.bindValue(":movie_id", newShowtime.getMovie().getId());
    query.bindValue(":start_at", newShowtime.getStartAt().toString("hh:mm:ss"));
    query.bindValue(":end_at", newShowtime.getEndAt().toString("hh:mm:ss"));
    query.bindValue(":show_date", newShowtime.getShowDate().toString("yyyy-MM-dd"));
    query.bindValue(":theater_id", newShowtime.getTheater().getId());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Showtime "  + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Showtime "  + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Showtime "+ queryType + " successful!");

    // Says that pbtSave was clicked
    emit is_pbtSave_clicked(true);
    close();

}

void ShowtimeAddModifyForm::on_pbtCancel_clicked()
{
    // Says that pbtSave wasn't clicked
    emit is_pbtSave_clicked(false);
    close();
}

// Sets the movie of the item that was double clicked.
// @param const QModelIndex &index. Double clicked item.
void ShowtimeAddModifyForm::on_tvwTable_doubleClicked(const QModelIndex &index)
{
    newShowtime.setMovie(Movie(index.sibling(index.row(),0).data().toInt(), index.sibling(index.row(),1).data().toString(),
                               index.sibling(index.row(),2).data().toInt(), QDate(), "", "", "", "", QDate(), QDate(), Country(), QList<MovieGenre>()));

    QString tmp = index.sibling(index.row(),1).data().toString();
    if(tmp.length() > 60)
    {
        tmp.chop(60);
        tmp += "...";
    }

    ui->lblMovie->setText(tmp);
}
