#include "castpositionsaddmodifyform.h"
#include "ui_castpositionsaddmodifyform.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This windows allows add or update an exisiting cast position.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

// Constructor for adding new cast position
CastPositionsAddModifyForm::CastPositionsAddModifyForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CastPositionsAddModifyForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    ui->lblId->hide();
    ui->lblIdlabel->hide();

    this->setWindowTitle("New Cast Position");

    queryType = "insertion";

}

// Constructor for updating existing position
CastPositionsAddModifyForm::CastPositionsAddModifyForm(QWidget *parent, CastPosition oldCastPosition) :
    QDialog(parent),
    ui(new Ui::CastPositionsAddModifyForm)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->oldCastPosition = oldCastPosition;

    ui->lblId->setText(QString::number(oldCastPosition.getId()));
    ui->ledName->setText(oldCastPosition.getPosition());

    this->setWindowTitle("Update Cast Position");

    queryType = "update";

}

CastPositionsAddModifyForm::~CastPositionsAddModifyForm()
{
    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Returns true if required fields are not blank
bool CastPositionsAddModifyForm::areFieldsNotBlank()
{
    if(ui->ledName->text() == "")
    {
        QMessageBox::warning(this, "Error", "Please fill following fields: Name!");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------------------------------------------------------
// Slots

void CastPositionsAddModifyForm::on_pbtOk_clicked()
{
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    // Return if user doesn't want to proceed.
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of cast position?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    CastPosition newCastPosition(ui->lblId->text().toInt(), ui->ledName->text());

    // Checks if new data is different from old data. If its the same, that means there were no changes made. Then return
    if(queryType == "update" && oldCastPosition.getPosition() == newCastPosition.getPosition())
    {
        QMessageBox::warning(this, "Update", "No changes made!");
        emit is_pbtOK_clicked(false);
        close();
        return;
    }

    if(conn.connOpen())
    {
        QSqlQuery query;

        if(queryType == "insertion")
        {
            query.prepare("INSERT into cast_position (position) "
                          "VALUES (:position);");
            query.bindValue(":position", newCastPosition.getPosition());
        }
        else
        {
            query.prepare("UPDATE cast_position "
                          "SET position = :position "
                          "WHERE cast_position_id = :id;");
            query.bindValue(":position", newCastPosition.getPosition());
            query.bindValue(":id", newCastPosition.getId());
        }

        if(query.exec())
        {
            QMessageBox::information(this, "Success", "Cast position \"" + newCastPosition.getPosition() + "\" " + queryType + " successful!");
        }
        else
        {
            QMessageBox::warning(this, "Error", "Cast position \"" + newCastPosition.getPosition() + "\" " + queryType + " failed!\n Error: " + query.lastError().nativeErrorCode());
        }

        conn.connClose();
        emit is_pbtOK_clicked(true);
        close();
    }
    else
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with cast position " + queryType +"!\n Error: " + conn.nativeErrorCode());
    }
}

void CastPositionsAddModifyForm::on_pbtCancel_clicked()
{
    emit is_pbtOK_clicked(false);
    close();
}
