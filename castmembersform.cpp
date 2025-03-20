#include "castmembersform.h"
#include "ui_castmembersform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window displays cast members. It also allows add new, delete or modify them.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

CastMembersForm::CastMembersForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CastMembersForm)
{
    ui->setupUi(this);
    //Prevents user from resizing window
    setWindowFlags(Qt::Window | Qt::MSWindowsFixedSizeDialogHint);

    model = 0;

    ui->ledFirstName->setPlaceholderText("Any");
    ui->ledMiddleName->setPlaceholderText("Any");
    ui->ledLastName->setPlaceholderText("Any");

    QStringList genderList = { "Any", "Male", "Female" };
    ui->cbxGender->addItems(genderList);

    ui->dedDOBTo->setDate(QDate::currentDate());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\n Error: " + conn.nativeErrorCode());
        close();
    }

    QSqlQuery query;

    query.prepare("SELECT * "
                  "FROM cast_position;");

    positionList.append("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        positionIDList.append(query.value(0).toInt());
        positionList.append(query.value(1).toString());
    }

    query.prepare("SELECT * "
                  "FROM country;");

    countryList.append("Any");

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't load cast positions!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    while(query.next())
    {
        countryIDList.append(query.value(0).toInt());
        countryList.append(query.value(1).toString());
    }

    ui->cbxPosition->addItems(positionList);
    ui->cbxCountry->addItems(countryList);

    conn.connClose();
}

CastMembersForm::~CastMembersForm()
{
    delete ui->tvwTable->model();
    if (model != 0)
        delete model;
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Load table with cast member information
bool CastMembersForm::loadTable()
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

    queryText = "SELECT cm.cast_member_id AS ID, cm.first_name AS 'First Name', cm.middle_name AS 'Middle Name', cm.last_name AS 'Last Name', "
                "cm.date_of_birth AS 'Date of Birth', c.name AS Country, cm.gender AS Gender, cp.position AS Position, c.country_id, cp.cast_position_id "
                "FROM country c "
                "INNER JOIN cast_member cm "
                "ON c.country_id = cm.country_id "
                "INNER JOIN cast_member__cast_position mp "
                "ON cm.cast_member_id = mp.cast_member_id "
                "INNER JOIN cast_position cp "
                "ON mp.cast_position_id = cp.cast_position_id "
                "WHERE 1 = 1 ";

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

    query.prepare( queryText + " ORDER BY cm.cast_member_id;");

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
        QMessageBox::warning(this, "Error", "Couldn't load cast members!\nError: " + query.lastError().nativeErrorCode());
        return false;
    }

    model->setQuery(query);

    QSortFilterProxyModel *m=new QSortFilterProxyModel(this);
    m->setDynamicSortFilter(true);
    m->setSourceModel(model);
    ui->tvwTable->setModel(m);
    ui->tvwTable->setSortingEnabled(true);
    ui->tvwTable->sortByColumn(0, Qt::AscendingOrder);
    ui->tvwTable->hideColumn(8);
    ui->tvwTable->hideColumn(9);

    conn.connClose();
    return true;

}

// Resets all the fields in the form
void CastMembersForm::resetFields()
{
    ui->ledFirstName->setText("");
    ui->ledMiddleName->setText("");
    ui->ledLastName->setText("");
    ui->cbxGender->setCurrentIndex(0);

    ui->dedDOBFrom->setEnabled(false);
    ui->dedDOBTo->setEnabled(false);
    ui->dedDOBFrom->setDate(QDate::currentDate());

    ui->cbxPosition->setCurrentIndex(0);
    ui->cbxCountry->setCurrentIndex(0);

    ui->chkDOB->setChecked(false);
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void CastMembersForm::on_pbtLoadTable_clicked()
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


void CastMembersForm::on_chkDOB_stateChanged(int checked)
{
    ui->dedDOBTo->setEnabled(checked);
    ui->dedDOBFrom->setEnabled(checked);
}

void CastMembersForm::on_pbtClearFields_clicked()
{
    resetFields();
}

void CastMembersForm::on_pbtAddNew_clicked()
{
    CastMembersAddModifyForm *addForm = new CastMembersAddModifyForm(this, countryIDList, countryList, positionIDList, positionList);

    connect (addForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    addForm->exec();
    delete addForm;
}

void CastMembersForm::on_pbtDelete_clicked()
{
    // Ask if user wants to delete cast member(s). If not return.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Deletion", "Proceed with deletion of cast member(s)?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Get rows that were selected
    QModelIndexList select = ui->tvwTable->selectionModel()->selectedRows();
    // Ids of cast members that will be deleted
    QVector<int> castIds(select.count());

   if(select.count() == 0)
   {
       QMessageBox::warning(this, "Error", "No rows selected!");
       return;
   }

   for(int i=0; i< select.count(); i++)
   {
       QModelIndex index = select.at(i);
       int row = index.row();
       castIds[i] = index.sibling(row,0).data().toInt();
   }

   // If there's an error while deleting cast members, it will be true
   bool error = false;

   if(!conn.connOpen())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete cast member(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   if(!conn.transaction())
   {
       QMessageBox::warning(this, "Error", "Couldn't delete cast member(s)!\nError: " + conn.nativeErrorCode());
       return;
   }

   // Delete all cast members that were selected
   for(int i=0; i< select.count(); i++)
   {
       QSqlQuery query;

       query.prepare("DELETE FROM cast_member "
                     "WHERE cast_member_id = :castId;");
       query.bindValue(":castId", castIds[i]);

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
       QMessageBox::information(this, "Success", "Cast member(s) deleted!");
       loadTable();
   }
}

void CastMembersForm::on_pbtModify_clicked()
{
    // Make sure only one row was selected
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

    CastMember cm;
    cm.setId(index.sibling(index.row(),0).data().toInt());
    cm.setFirstName(index.sibling(index.row(),1).data().toString());
    cm.setMiddleName(index.sibling(index.row(),2).data().toString());
    cm.setLastName(index.sibling(index.row(),3).data().toString());
    cm.setDOB(index.sibling(index.row(),4).data().toString());
    cm.setCountry(Country(index.sibling(index.row(),8).data().toInt(),
                           index.sibling(index.row(),5).data().toString()));
    cm.setGender(index.sibling(index.row(),6).data().toString());

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't open Movie form!\nError: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    // Get position(s) of cast member
    query.prepare("SELECT cp.cast_position_id, cp.position "
                  "FROM cast_position cp "
                  "WHERE cp.cast_position_id IN ( "
                  "SELECT mp.cast_position_id "
                  "FROM cast_member__cast_position mp "
                  "WHERE mp.cast_member_id = :id);");

    query.bindValue(":id", cm.getId());

    if(!query.exec())
    {
        QMessageBox::warning(this, "Error", "Couldn't open Cast Member form!\nError: " + query.lastError().nativeErrorCode());
        return;
    }

    while(query.next())
    {
        cm.appendPosition(CastPosition(query.value(0).toInt(), query.value(1).toString()));
    }

    CastMembersAddModifyForm *modifyForm = new CastMembersAddModifyForm(this, countryIDList, countryList, positionIDList, positionList, cm);

    connect (modifyForm, SIGNAL (is_pbtOK_clicked(bool)), this, SLOT(on_is_pbtOK_clicked(bool)));
    modifyForm->exec();
    delete modifyForm;
}

// Executes when push button "pbtOK" from CastMembersAddModifyForm was clicked
void CastMembersForm::on_is_pbtOK_clicked(bool clicked)
{
    if(clicked)
        loadTable();
}

