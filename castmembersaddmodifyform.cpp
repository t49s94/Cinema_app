#include "castmembersaddmodifyform.h"
#include "ui_castmembersaddmodifyform.h"

#include "login.h"
#include <qmessagebox.h>
#include "castposition.h"
#include "castmember.h"

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

// For adding a new cast member
CastMembersAddModifyForm::CastMembersAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList,
                                                   QList<int> positionIDList, QStringList positionList) :
    QDialog(parent),
    ui(new Ui::CastMembersAddModifyForm)
{
    ui->setupUi(this); 

    // Removes "Any" item
    positionList.removeAt(0);
    // Fill lwdPosition with checkable items
    for(int i = 0; i < positionList.length(); i++)
    {
        ui->lwdPosition->addItem(positionList.at(i));
        QListWidgetItem* item = ui->lwdPosition->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
    }

    initializeForm(&countryIDList, &countryList, &positionIDList);

    ui->lblId->hide();
    ui->lblIdlabel->hide();
    this->setWindowTitle("New Cast Member");
    queryType = "insertion";
}

// For updating an existing cast member
CastMembersAddModifyForm::CastMembersAddModifyForm(QWidget *parent, QList<int> countryIDList, QStringList countryList,
                                                   QList<int> positionIDList, QStringList positionList, CastMember oldMember) :
    QDialog(parent),
    ui(new Ui::CastMembersAddModifyForm)
{
    ui->setupUi(this);

    this->oldMember = oldMember;

    // Removes "Any" item
    positionList.removeAt(0);

    // Copy posistionList. We remove an item each time there's a match in positionList. We do it until we empty the QList.
    QList<CastPosition> positions = oldMember.getPositionsList();

    // Fill lwdPosition with checkable items
    for(int i = 0; i < positionList.length(); i++)
    {
        ui->lwdPosition->addItem(positionList.at(i));
        QListWidgetItem* item = ui->lwdPosition->item(i);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);

        bool isPositionChecked = false;
        // Loop thru the Cast positions that the cast member has. If He has the current Cast position, we check the item.
        for(int j = 0; j < positions.count(); j++)
        {
            if(positions[j].getPosition() == item->text())
            {
                item->setCheckState(Qt::Checked);
                oldCheckedPositionIdxList.append(i);
                isPositionChecked = true;
                positions.takeAt(j);
                break;
            }
        }

        // We have to set checkstate as checked or unchecked that's why we ask if item was checked. If not, set checkstate as unchecked
        if (!isPositionChecked)
            item->setCheckState(Qt::Unchecked);

    }

    initializeForm(&countryIDList, &countryList, &positionIDList);

    ui->lblId->setText(QString::number(oldMember.getId()));
    ui->ledFirstName->setText(oldMember.getFirstName());
    ui->ledMiddleName->setText(oldMember.getMiddleName());
    ui->ledLastName->setText(oldMember.getLastName());
    ui->dedDOB->setDate(oldMember.getDOB());
    ui->cbxCountry->setCurrentText(oldMember.getCountry().getName());
    ui->cbxGender->setCurrentText(oldMember.getGender() == "M" ? "Male" : "Female");

    this->setWindowTitle("Update Cast Member");
    queryType = "update";
}

CastMembersAddModifyForm::~CastMembersAddModifyForm()
{
    delete ui;
}

//------------------------------------------------------------------------------------------------------------------------------
// Functions

void CastMembersAddModifyForm::initializeForm(QList<int>* countryIDList, QStringList* countryList,  QList<int>* positionIDList)
{
    // Prevents user to resize window
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Removes "Any" item
    countryList->removeAt(0);
    this->countryIDList = *countryIDList;

    ui->cbxCountry->addItems(*countryList);
    ui->cbxGender->addItems({ "Male", "Female" });

    // When item is checked from lwdPosition, we highlight it
    QObject::connect(ui->lwdPosition, SIGNAL(itemChanged(QListWidgetItem*)),
                         this, SLOT(highlightChecked(QListWidgetItem*)));

    this->positionIDList = *positionIDList;
}

bool CastMembersAddModifyForm::addCastMember(QSqlQuery* query, CastMember* newMember)
{
    query->prepare("INSERT into cast_member (first_name, middle_name, last_name, date_of_birth, country_id, gender) "
                  "VALUES (:firstName, :middleName, :lastName, :dob, :countryID, :gender);");
    query->bindValue(":firstName", newMember->getFirstName());
    query->bindValue(":middleName", newMember->getMiddleName());
    query->bindValue(":lastName", newMember->getLastName());
    query->bindValue(":dob", newMember->getDOB());
    query->bindValue(":countryID", newMember->getCountry().getId());
    query->bindValue(":gender", newMember->getGender());

    if(!query->exec())
    {
        conn.connClose();

        QMessageBox::warning(this, "Error", "Cast member \"" + newMember->getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query->lastError().nativeErrorCode());
        return false;
    }

    query->prepare("SELECT LAST_INSERT_ID();");
    query->exec();

    if(!query->next())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Cast member \"" + newMember->getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query->lastError().nativeErrorCode());
        return false;
    }

    int insertedID = query->value(0).toInt();

    // We insert a new row in cast_member__cast_position for each position we added to cast member
    foreach(CastPosition position, newMember->getPositionsList())
    {
        query->prepare("INSERT INTO cast_member__cast_position (cast_member_id, cast_position_id) "
                      "VALUES (:insertedID, :positionID);");
        query->bindValue(":insertedID", insertedID);
        query->bindValue(":positionID", position.getId());

        if(!query->exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Cast member \"" + newMember->getFullName() + "\" " + queryType +
                                 " failed!\n Error: " + query->lastError().nativeErrorCode());
            return false;
        }
    }

    return true;
}

bool CastMembersAddModifyForm::updateCastMember(QSqlQuery *query, CastMember *newMember, QList<int>* newCheckedPositionIdxList)
{
    // Copy oldCheckedPositionIdxList to remove the cast positions that are found. We do this to optimize search and don't look for
    // positions that have been found already. Also to determine which positions have been added or deleted.
    QList<int> auxOldChecked = oldCheckedPositionIdxList;
    // if lists are different. Means that user updated Cast Positions list
    if(oldCheckedPositionIdxList != *newCheckedPositionIdxList)
    {
        // Loop thru newCheckedPositionIdxList to determine the position that both lists have in common. If there's a position in newCheckedPositionIdxList
        // that isn't in auxOldChecked means that user added a new position. So we insert those positions to cast_member__cast_position table.
        // If there's a position that auxOldChecked has and newCheckedPositionIdxList not means user deleted that position. So we have delete it from
        // cast_member__cast_position table.
        for(int i = 0; i < newCheckedPositionIdxList->count(); i++)
        {
            bool positionFound = false;
            for(int j = 0; j < auxOldChecked.count(); j++)
                if(newCheckedPositionIdxList->at(i) == auxOldChecked.at(j))
                {
                    positionFound = true;
                    // remove the position found from list
                    auxOldChecked.removeAt(j);
                    break;
                }

            // If position wasn't found, we insert the position in the cast_member__cast_position table
            if(!positionFound)
            {
                query->prepare("INSERT INTO cast_member__cast_position (cast_member_id, cast_position_id) "
                              "VALUES (:insertedID, :positionID);");
                query->bindValue(":insertedID", newMember->getId());
                query->bindValue(":positionID", newMember->getPositionAt(i).getId());

                if(!query->exec())
                {
                    conn.connClose();
                    QMessageBox::warning(this, "Error", "Cast member \"" + newMember->getFullName() + "\" " + queryType +
                                         " failed!\n Error: " + query->lastError().nativeErrorCode());
                    return false;
                }
            }

        }

        // Loop thru the rest of auxOldChecked list. These positions are the ones the user deleted so we remove them from
        // cast_member__cast_position table
        for(int i = 0; i < auxOldChecked.count(); i++)
        {
            query->prepare("DELETE FROM cast_member__cast_position "
                          "WHERE cast_member_id = :castMemberID AND cast_position_id = :castPositionID");
            query->bindValue(":castMemberID", oldMember.getId());
            query->bindValue(":castPositionID", positionIDList.at(auxOldChecked.at(i)));

            if(!query->exec())
            {
                conn.connClose();
                QMessageBox::warning(this, "Error", "Cast member \"" + newMember->getFullName() + "\" " + queryType +
                                     " failed!\n Error: " + query->lastError().nativeErrorCode());
                return false;
            }
        }

    }

    QString queryText = "UPDATE cast_member ";
    // Contains the fields that were updated by the user
    QStringList setsList;

    if(oldMember.getFirstName() != newMember->getFirstName())
        setsList.append(", first_name = :firstName ");
    if(oldMember.getMiddleName() != newMember->getMiddleName())
        setsList.append(", middle_name = :middleName ");
    if(oldMember.getLastName() != newMember->getLastName())
        setsList.append(", last_name = :lastName ");
    if(oldMember.getDOB() != newMember->getDOB())
        setsList.append(", date_of_birth = :dob ");
    if(oldMember.getCountry().getId() != newMember->getCountry().getId())
        setsList.append(", country_id = :countryID ");
    if(oldMember.getGender() != newMember->getGender())
        setsList.append(", gender = :gender ");

    // If user updated at least a field
    if(setsList.length() > 0)
    {
        for(int i = 0; i < setsList.length(); i++)
            queryText += setsList.at(i);

        queryText += " WHERE cast_member_id = :id;";

        QString subStr(","); // String to replace.
        QString newStr("SET"); // Replacement string.
        queryText.replace(queryText.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

        query->prepare(queryText);

        query->bindValue(":id", newMember->getId());
        query->bindValue(":firstName", newMember->getFirstName());
        query->bindValue(":middleName", newMember->getMiddleName());
        query->bindValue(":lastName", newMember->getLastName());
        query->bindValue(":dob", newMember->getDOB());
        query->bindValue(":countryID", newMember->getCountry().getId());
        query->bindValue(":gender", newMember->getGender());

        if(!query->exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Cast member \"" + newMember->getFullName() + "\" " + queryType +
                                 " failed!\n Error: " + query->lastError().nativeErrorCode());
            return false;
        }

    }

    return true;
}

// Returns true if fields required fields are not blank
// @returns bool that says if required fields are blank or not
bool CastMembersAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->ledFirstName->text() == "")
        message += ", first name";
    if(ui->ledLastName->text() == "")
        message += ", last name";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;
}

//------------------------------------------------------------------------------------------------------------------------------
// Slots

// When item is checked from lwdPosition, we highlight it
void CastMembersAddModifyForm::highlightChecked(QListWidgetItem *item)
{
    if(item->checkState() == Qt::Checked)
        item->setBackgroundColor(QColor("#ffffb2"));
    else
        item->setBackgroundColor(QColor("#ffffff"));
}

void CastMembersAddModifyForm::on_pbtOk_clicked()
{
    QString action = queryType;
    action[0] = action[0].toUpper();
    // We ask if the user wants to proceed with add/update of cast member. If not, return
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of employee position?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    // Check if required fields are blank
    if(!areFieldsNotBlank())
        return;

    CastMember newMember;

    newMember.setId(ui->lblId->text().toInt());
    newMember.setFirstName(ui->ledFirstName->text());
    newMember.setMiddleName(ui->ledMiddleName->text());
    newMember.setLastName(ui->ledLastName->text());
    newMember.setDOB(ui->dedDOB->date());
    newMember.setCountry(Country(countryIDList.at(ui->cbxCountry->currentIndex()),
                                 ui->cbxCountry->currentText()));
    newMember.setGender(ui->cbxGender->currentText() == "Male" ? "M" : "F");

    QList<CastPosition> positionList;
    QList<int> newCheckedPositionIdxList;
    // We get the indexes of items that were checked
    for(int i = 0; i < ui->lwdPosition->count(); i++)
    {
        if(ui->lwdPosition->item(i)->checkState() == Qt::Checked)
        {
            positionList.append(CastPosition(positionIDList.at(i), ui->lwdPosition->item(i)->text()));
            newCheckedPositionIdxList.append(i);
        }

    }

    newMember.setPositionsList(positionList);

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

    QSqlQuery query;

    if(queryType == "insertion")
    {
        if(!addCastMember(&query, &newMember))
            return;
    }
    else
        if(!updateCastMember(&query, &newMember, &newCheckedPositionIdxList))
            return;

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Cast member \"" + newMember.getFullName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Cast member \"" + newMember.getFullName() + "\" " + queryType + " successful!");
    emit is_pbtOK_clicked(true);
    close();

}

void CastMembersAddModifyForm::on_pbtCancel_clicked()
{
    emit is_pbtOK_clicked(false);
    close();
}
