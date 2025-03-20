#include "moviecastaddmodifyform.h"
#include "ui_moviecastaddmodifyform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows add new or update list of cast that works in a movie.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MovieCastAddModifyForm::MovieCastAddModifyForm(QWidget *parent, QStringList countryList, QStringList positionList, Movie movie) :
    QDialog(parent),
    ui(new Ui::MovieCastAddModifyForm)
{
    ui->setupUi(this);
    // Prevent user from resizing window.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledFirstName->setPlaceholderText("Any");
    ui->ledMiddleName->setPlaceholderText("Any");
    ui->ledLastName->setPlaceholderText("Any");

    QStringList genderList = { "Any", "Male", "Female" };
    ui->cbxGender->addItems(genderList);

    ui->dedDOBTo->setDate(QDate::currentDate());

    this->movie = movie;

    ui->cbxCountry->addItems(countryList);
    ui->cbxPosition->addItems(positionList);

    ui->lblTitle->setText("ADD CAST MEMBERS TO \"" +  movie.getTitle().toUpper() + "\"");
    this->setWindowTitle("Add Cast Members");

}

MovieCastAddModifyForm::~MovieCastAddModifyForm()
{
    if(model != 0)
        delete model;
    delete ui->tvwTable->model();

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with cast members so the user can select the ones who will work in the movie.
// @return bool. True if table was successfully loaded.
bool MovieCastAddModifyForm::loadTable()
{
    delete model;
    delete ui->tvwTable->model();
    model = new QSqlQueryModel();

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\n Error: " + conn.nativeErrorCode());
        return false;
    }

    QSqlQuery query;
    QString queryText;

    queryText = "SELECT cm.cast_member_id AS ID, cm.first_name AS 'First Name', cm.middle_name AS 'Middle Name', cm.last_name AS 'Last Name', "
                "cm.date_of_birth AS 'Date of Birth', c.name AS Country, cm.gender AS Gender, cp.position AS Position, c.country_id, cp.cast_position_id, mp.id "
                "FROM cast_member__cast_position mp "
                "INNER JOIN cast_member cm "
                "ON mp.cast_member_id = cm.cast_member_id "
                "INNER JOIN country c "
                "ON cm.country_id = c.country_id "
                "INNER JOIN cast_position cp "
                "ON mp.cast_position_id = cp.cast_position_id "
                "LEFT JOIN movie__cast_member mc "
                "ON mc.cast_member__cast_position_id = mp.id "
                "WHERE (mc.movie_id != :movieID OR mc.movie_id IS NULL) ";

    //----------------------------------------------------------------------------------------------------------------------------
    // Appends the filters to the query

    QString firstName = ui->ledFirstName->text();
    queryText += firstName != ""? " AND cm.first_name = :firstName" : "";

    QString middleName = ui->ledMiddleName->text();
    queryText += middleName != ""? " AND cm.middle_name = :middleName" : "";

    QString lastName = ui->ledLastName->text();
    queryText += lastName != ""? " AND cm.last_name = :lastName" : "";

    QString dobFrom = ui->dedDOBFrom->date().toString("yyyy-M-d");
    QString dobTo = ui->dedDOBTo->date().toString("yyyy-M-d");
    queryText += ui->chkDOB->isChecked()? " AND cm.date_of_birth >= :dobFrom AND cm.date_of_birth <= :dobTo" : "";

    QString country = ui->cbxCountry->currentText();
    queryText += country != "Any"? " AND c.name = :country" : "";

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

    queryText += " ORDER BY cm.cast_member_id;";

    query.prepare(queryText );

    query.bindValue(":movieID", movie.getId());
    query.bindValue(":firstName", firstName);
    query.bindValue(":middleName", middleName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":dobFrom", dobFrom);
    query.bindValue(":dobTo", dobTo);
    query.bindValue(":country", country);
    query.bindValue(":gender", gender);
    query.bindValue(":position", position);

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast members!\nError: " + query.lastError().nativeErrorCode() + query.lastError().text());
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
    // Hides c.country_id, cp.cast_position_id, mp.id
    ui->tvwTable->hideColumn(8);
    ui->tvwTable->hideColumn(9);
    ui->tvwTable->hideColumn(10);

    conn.connClose();
    return true;

}

// Reset fields in form
void MovieCastAddModifyForm::resetFields()
{
    ui->ledFirstName->setText("");
    ui->ledMiddleName->setText("");
    ui->ledLastName->setText("");
    ui->cbxGender->setCurrentIndex(0);

    ui->dedDOBFrom->setDate(QDate::currentDate());

    ui->cbxPosition->setCurrentIndex(0);
    ui->cbxCountry->setCurrentIndex(0);

    ui->chkDOB->setChecked(false);
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void MovieCastAddModifyForm::on_pbtLoadTable_clicked()
{
    if(loadTable())
        ui->pbtOk->setEnabled(true);
    else
        ui->pbtOk->setEnabled(false);
}


void MovieCastAddModifyForm::on_chkDOB_stateChanged(int checked)
{
    ui->dedDOBTo->setEnabled(checked);
    ui->dedDOBFrom->setEnabled(checked);
}

void MovieCastAddModifyForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void MovieCastAddModifyForm::on_pbtOk_clicked()
{
    // Asks if user wants to proceed. Return if not.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Insertion", "Proceed with insertion of cast member(s) to movie \"" + movie.getTitle() +"\"?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    if(ui->tvwTable->selectionModel()->selectedRows().count() < 1)
    {
        QMessageBox::warning(this, "Error", "Please select at least 1 row!");
        return;
    }

    // Get selected rows
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // List with Ids of cast members that are gonna be added to movie.
    QList<int> castIds;

    // Populate QList castIds
    for(int i=0; i< select.count(); i++)
    {
        QModelIndex index = select.at(i);
        int row = index.row();
        castIds.append(index.sibling(row,10).data().toInt());
    }

   // Says if there was an error while adding cast members to the movie
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't add cast member(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't add cast member(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Insert cast members to the movie
   for(int i=0; i< castIds.length(); i++)
   {
       QSqlQuery query;

       query.prepare("INSERT INTO movie__cast_member "
                     "VALUES (:movieID, :castID);");
       query.bindValue(":movieID", movie.getId());
       query.bindValue(":castID", castIds.at(i));

       if(!query.exec())
       {
           conn.connClose();
           error = true;
           QMessageBox::warning(this, "Error", "Couldn't add cast member(s)!\nError: " + query.lastError().nativeErrorCode());
           break;
       }

   }

   if(!error)
   {
       if(!conn.commit())
       {
           conn.connClose();
           QMessageBox::warning(this, "Error", "Couldn't add cast member(s)!\nError: " + conn.nativeErrorCode());
           return;
       }

       conn.connClose();
       QMessageBox::information(this, "Success", "Cast member(s) added!");
   }

    // Means that pbtOk was clicked.
    emit is_pbtOK_clicked(true);
    close();

}

void MovieCastAddModifyForm::on_pbtCancel_clicked()
{
    // Means that pbtOk wasn't clicked.
    emit is_pbtOK_clicked(false);
    close();
}



