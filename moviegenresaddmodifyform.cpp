/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing movie genre.

*/

#include "moviegenresaddmodifyform.h"
#include "ui_moviegenresaddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding a new movie genre.
MovieGenresAddModifyForm::MovieGenresAddModifyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MovieGenresAddModifyForm)
{
    ui->setupUi(this);
    // Prevents user from resizing dialog.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lblId->hide();
    ui->lblIdlabel->hide();

    this->setWindowTitle("New Movie Genre");

    queryType = "insertion";
}

// Constructor for updating existing movie genre.
MovieGenresAddModifyForm::MovieGenresAddModifyForm(QWidget *parent, MovieGenre oldGenre) :
    QDialog(parent),
    ui(new Ui::MovieGenresAddModifyForm)
{
    ui->setupUi(this);
    // Prevents user from resizing dialog.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->oldGenre = oldGenre;
    ui->lblId->setText(QString::number(oldGenre.getId()));
    ui->ledGenre->setText(oldGenre.getGenre());

    this->setWindowTitle("Update Movie Genre");

    queryType = "update";
}

MovieGenresAddModifyForm::~MovieGenresAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void MovieGenresAddModifyForm::on_pbtOk_clicked()
{
    QString action = queryType;
    action[0] = action[0].toUpper();
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of movie genre?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    MovieGenre newGenre;
    newGenre.setId(ui->lblId->text().toInt());
    newGenre.setGenre(ui->ledGenre->text());

    if(queryType == "update" && oldGenre.getGenre() == newGenre.getGenre())
    {
        QMessageBox::warning(this, "Update", "No changes made!");
        emit is_pbtOK_clicked(false);
        close();
        return;
    }

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with movie genre " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    if(queryType == "insertion")
        query.prepare("INSERT into movie_genre (genre) "
                      "VALUES (:genre);");
    else
        query.prepare("UPDATE movie_genre "
                      "SET genre = :genre "
                      "WHERE movie_genre_id = :id;");

    query.bindValue(":genre", newGenre.getGenre());
    query.bindValue(":id", newGenre.getId());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Movie genre \"" + newGenre.getGenre() + "\" " +
                              queryType + " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Movie genre \"" + newGenre.getGenre() + "\" " + queryType + " successful!");

    // Means push button pbtOk was clicked.
    emit is_pbtOK_clicked(true);
    close();

}

void MovieGenresAddModifyForm::on_pbtCancel_clicked()
{
    // Means push button pbtOk wasn't clicked.
    emit is_pbtOK_clicked(false);
    close();
}
