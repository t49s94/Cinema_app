/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user to add new or update existing theate.

*/

#include "theateraddmodifyform.h"
#include "ui_theateraddmodifyform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

// Constructor for adding new theater
TheaterAddModifyForm::TheaterAddModifyForm(QWidget *parent, QList<QList<QString>> theaterTypesList) :
    QDialog(parent),
    ui(new Ui::TheaterAddModifyForm)
{
    ui->setupUi(this);

    initialize(&theaterTypesList);

    ui->lblId->hide();
    ui->lbllblId->hide();
    this->setWindowTitle("New Theater");
    queryType = "insertion";
}

// Constructor for updating existing theater
TheaterAddModifyForm::TheaterAddModifyForm(QWidget *parent, QList<QList<QString>> theaterTypesList, Theater oldTheater) :
    QDialog(parent),
    ui(new Ui::TheaterAddModifyForm)
{
    ui->setupUi(this);

    initialize(&theaterTypesList);

    this->oldTheater = oldTheater;

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't proceed with " + queryType +"!\n Error: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    query.prepare("SELECT * FROM seat "
                  "WHERE theater_id = :theater_id;");

    query.bindValue(":theater_id", oldTheater.getId());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load seats!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    int i = -1;
    // Creates the seats of theater
    while(query.next())
    {
        QString row = query.value(2).toString();
        oldSeatList.append(Seat(query.value(0).toInt(),
                                Theater(oldTheater.getId(), "", TheaterType()),
                                row.toStdString().c_str()[0], query.value(3).toString(),
                                query.value(4).toBool(), query.value(5).toInt(),
                                query.value(6).toInt()));
        oldSeatIdNumberList.append(++i);

        QString imagePath = query.value(4).toBool() ? ":/Images/available handicap seat.png" :
                                                     ":/Images/available seat.png";

        addSeat(imagePath, QPoint(query.value(5).toInt(), query.value(6).toInt()), row + query.value(3).toString());

    }

    conn.connClose();

    ui->lblId->setText(QString::number(oldTheater.getId()));
    ui->ledName->setText(oldTheater.getName());
    ui->cbxTheaterType->setCurrentText(oldTheater.getTheaterType().getName());

    this->setWindowTitle("Update Theater");
    queryType = "update";
}

TheaterAddModifyForm::~TheaterAddModifyForm()
{
    delete dragWidget;
    delete horizontalLayout;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initializes some features of form.
// @param QList<QList<QString>> *theaterTypesList. 2D List with "Id" and "Theater type name".
void TheaterAddModifyForm::initialize(QList<QList<QString>> *theaterTypesList)
{
    // Prevents user from resizing window.
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    // Delete "Any" item.
    theaterTypesList->removeAt(0);
    this->theaterTypesList = *theaterTypesList;
    for(int i = 0; i < theaterTypesList->length(); i++)
        ui->cbxTheaterType->addItem(theaterTypesList->at(i).at(1));

    // Only allows 1 alphabetical character
    ui->ledSeatRow->setInputMask("a");
    // Connects signal pressed which is emitted when ledSeatRow is clicked to slot on_ledSeatRow_pressed.
    connect (ui->ledSeatRow, SIGNAL (pressed()), this, SLOT(on_ledSeatRow_pressed()));

    horizontalLayout = new QHBoxLayout(ui->widTheater);
    dragWidget = new DragWidget();
    connect (dragWidget, SIGNAL (is_pressed(const QString, const QPoint&, SeatWidget&)), this, SLOT(on_seat_clicked(const QString, const QPoint&, SeatWidget&)));

    horizontalLayout->addWidget(dragWidget);
    // Means no seat has been selected
    currentIdNumber = -1;

    QPixmap pixmap(":/Images/movie theater.png");
    ui->lblTheater->setPixmap(pixmap);
}

// Checks if required fields were filled out.
// @return bool. True if all required fields were filled out.
bool TheaterAddModifyForm::areFieldsNotBlank()
{
    QString message = "Please fill following fields:";

    if(ui->ledName->text() == "")
        message += " Name";

    /*
    if(ui->spbDiscount->value() == 0)
        message += ", Discount";

    QString subStr(","); // String to replace.
    QString newStr(""); // Replacement string.
    message.replace(message.indexOf(subStr), subStr.size(), newStr); //replace first occurrence
    */

    if(message == "Please fill following fields:")
        return true;
    else
        QMessageBox::warning(this, "Error", message + "!");

    return false;

}

// Draws a new seat in theater.
// @param QString imagePath. Background image of seat.
// @param QPoint pos. Position where seat is located.
void TheaterAddModifyForm::addSeat(QString imagePath, QPoint pos)
{
    SeatWidget* seatWidget = new SeatWidget(dragWidget, 0, new Seat(), imagePath);
    // We label this seatWidget as "newSeat" to know this is a brand new seat
    seatWidget->setObjectName("newSeat");

    if(imagePath != ":/Images/available seat.png")
        seatWidget->getSeat()->setHandicap(true);

    dragWidget->addImage(seatWidget, imagePath, pos);

}

// Draws an existing seat in theater
// @param QString imagePath. Background image of seat.
// @param QPoint pos. Position where seat is located.
void TheaterAddModifyForm::addSeat(QString imagePath, QPoint pos, QString text)
{
    SeatWidget* seatWidget = new SeatWidget(dragWidget, 0, &oldSeatList.last(), imagePath);
    // We label this seatWidget as "oldSeat" to know this seat existed already.
    seatWidget->setObjectName("oldSeat");

    if(imagePath != ":/Images/available seat.png")
        seatWidget->getSeat()->setHandicap(true);

    dragWidget->addImage(seatWidget, imagePath, pos, text);
}

// Saves seats in theater.
// @param QSqlQuery *query.
bool TheaterAddModifyForm::saveSeats(QSqlQuery *query)
{
    int theaterId;
    // Get theaterId
    if(queryType == "update")
    {
        theaterId =  ui->lblId->text().toInt();
    }
    else
    {
        query->prepare("SELECT LAST_INSERT_ID();");

        if(!query->exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Theater \"" + newTheater.getName() + "\" " + queryType +
                                 " failed!\n Error: " + query->lastError().nativeErrorCode());
            return false;
        }

        while (query->next())
            theaterId = query->value(0).toInt();
    }

    // Gets the seats from dragWidget.
    QList<SeatWidget*> seatsList = dragWidget->findChildren<SeatWidget*>();

    // Makes sure there's at least one seat.
    if(seatsList.length() == 0)
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Theaters mustn't be empty. Please add seats!" );
        return false;
    }

    // Makes sure there are no repeated seats. Example: "D2" and "D2".
    for(int i = 0; i < seatsList.length(); i++)
    {
        QString rowColumn = seatsList[i]->getSeat()->getSeatRow() + seatsList[i]->getSeat()->getSeatColumn();
        for(int j = i + 1; j < seatsList.length(); j++)
        {
            QString nextRowColumn = seatsList[j]->getSeat()->getSeatRow() + seatsList[j]->getSeat()->getSeatColumn();
            if(rowColumn == nextRowColumn)
            {
                conn.connClose();
                QMessageBox::warning(this, "Error", "Repeated seat: " + nextRowColumn + "!" );
                return false;
            }
        }
    }

    QString queryText = "";
    // Add new or update existing seat to DB
    for(int i = 0; i < seatsList.length(); i++)
    {
        if(seatsList[i]->getSeat()->getSeatRow() == ' ' || seatsList[i]->getSeat()->getSeatColumn() == "")
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Please input row and column for each seat!" );
            return false;
        }

        if(seatsList[i]->objectName() == "newSeat")
            queryText = "INSERT INTO seat (theater_id, seat_row, seat_column, handicap, position_x, position_y) "
                        "VALUES (:theater_id, :seat_row, :seat_column, :handicap, :position_x, :position_y); ";
        else if(seatsList[i]->objectName() == "modifiedSeat" || seatsList[i]->x() != seatsList[i]->getSeat()->getPositionX()
                || seatsList[i]->y() != seatsList[i]->getSeat()->getPositionY())
            queryText = "UPDATE seat "
                        "SET seat_row = :seat_row, seat_column = :seat_column, position_x = :position_x, position_y = :position_y "
                        "WHERE seat_id = :seat_id; ";
        else
            continue;


        query->prepare(queryText);

        query->bindValue(":seat_id", seatsList.at(i)->getSeat()->getId());
        query->bindValue(":theater_id", theaterId);
        query->bindValue(":seat_row", QChar(seatsList.at(i)->getSeat()->getSeatRow()) + QString(""));
        query->bindValue(":seat_column", seatsList.at(i)->getSeat()->getSeatColumn());
        query->bindValue(":handicap", seatsList.at(i)->getSeat()->getHandicap());
        query->bindValue(":position_x", seatsList.at(i)->pos().x());
        query->bindValue(":position_y", seatsList.at(i)->pos().y());

        if(!query->exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Theater \"" + newTheater.getName() + "\" " + queryType +
                                 " failed!\n Error: " + query->lastError().nativeErrorCode());
            return false;
        }
    }

    // Delete seats in removedSeatsList from DB
    for (Seat removedSeat : removedSeatsList)
    {
        queryText = "DELETE FROM seat "
                    "WHERE seat_id = :seat_id; ";

        query->prepare(queryText);
        query->bindValue(":seat_id", removedSeat.getId());

        if(!query->exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Theater \"" + newTheater.getName() + "\" " + queryType +
                                 " failed!\n Error: " + query->lastError().nativeErrorCode());
            return false;
        }

    }

    return true;
}

// Creates query text for update.
// @param QString* queryText. QString that contains update query text
// @return bool. False if there were no changes made.
bool TheaterAddModifyForm::setUpdateQueryText(QString *queryText, QStringList *setsList)
{
    *queryText = "UPDATE theater ";

    if(oldTheater.getName() != newTheater.getName())
        setsList->append(", name = :name ");
    if(oldTheater.getTheaterType().getName() != newTheater.getTheaterType().getName())
        setsList->append(", theater_type_id = :theater_type_id ");

    if(setsList->length() == 0)
    {
        *queryText = "";
    }
    else
    {
        for(int i = 0; i < setsList->length(); i++)
            *queryText += setsList->at(i);

        *queryText += " WHERE theater_id = :theater_id;";

        QString subStr(","); // String to replace.
        QString newStr("SET"); // Replacement string.
        queryText->replace(queryText->indexOf(subStr), subStr.size(), newStr); //replace first occurrence
    }

    return true;
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void TheaterAddModifyForm::on_pbtSave_clicked()
{
    // Makes sure all required fields were filled out.
    if(!areFieldsNotBlank())
        return;

    QString action = queryType;
    action[0] = action[0].toUpper();
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, action, "Proceed with " + queryType + " of theater?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    newTheater.setId(ui->lblId->text().toInt());
    newTheater.setName(ui->ledName->text());
    newTheater.setTheaterType(
        TheaterType(theaterTypesList[ui->cbxTheaterType->currentIndex()][0].toInt(),ui->cbxTheaterType->currentText(), 0, 0)
    );

    QString queryText = "";

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

    QSqlQuery query;
    QStringList setsList;

    if(queryType == "insertion")
        queryText = "INSERT INTO theater (name, theater_type_id) "
                    "VALUES (:name, :theater_type_id); ";
    else
        setUpdateQueryText(&queryText, &setsList);

    query.prepare(queryText);

    query.bindValue(":theater_id", newTheater.getId());
    query.bindValue(":name", newTheater.getName());
    query.bindValue(":theater_type_id", newTheater.getTheaterType().getId());

    // If user want to create new theater or wants to update at least one field of existing theater.
    if(queryType == "insertion" || setsList.length() > 0)
        if(!query.exec())
        {
            conn.connClose();
            QMessageBox::warning(this, "Error", "Theater \"" + newTheater.getName() + "\" " + queryType +
                                 " failed!\n Error: " + query.lastError().nativeErrorCode());
            return;
        }

    // Return if an error occurred while saving seats.
    if(!saveSeats(&query))
        return;

    if(!conn.commit())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Theater \"" + newTheater.getName() + "\" " + queryType +
                             " failed!\n Error: " + query.lastError().nativeErrorCode());
        return;
    }

    conn.connClose();
    QMessageBox::information(this, "Success", "Theater \"" + newTheater.getName() + "\" " + queryType + " successful!");

    // Says pbtSave was clicked
    emit is_pbtSave_clicked(true);
    close();

}

void TheaterAddModifyForm::on_pbtCancel_clicked()
{
    // Says pbtSave wasn't clicked
    emit is_pbtSave_clicked(false);
    close();
}

void TheaterAddModifyForm::on_pbtAddSeat_clicked()
{
    addSeat(":/Images/available seat.png", QPoint(10, 10));
}

// Executed when user clicks on seat.
// @param QString buttonPressed. Button that was pressed.
// @param const QPoint &pos. Position where mouse was.
// @param SeatWidget &seatWidget. Seat that was clicked
void TheaterAddModifyForm::on_seat_clicked(const QString buttonPressed, const QPoint &pos, SeatWidget &seatWidget)
{
    if(buttonPressed == "left")
    {
        currentIdNumber = seatWidget.getIdNumber();
        currentSeat = *seatWidget.getSeat();
        // Keeps track of seat user clicked before the current one.
        dragWidget->setPreviousIdNumber(currentIdNumber);

        if(currentSeat.getSeatRow() == ' ')
            ui->ledSeatRow->setText("");
        else
            ui->ledSeatRow->setText(QString(QChar::fromLatin1(currentSeat.getSeatRow())));
        ui->spbSeatColumn->setValue(currentSeat.getSeatColumn().toInt());
        ui->ledSeatRow->setEnabled(true);
        ui->spbSeatColumn->setEnabled(true);
        ui->pbtModifyRowColumn->setEnabled(true);
        ui->pbtRemoveSeat->setEnabled(true);

    }
    else
    {
        ui->ledSeatRow->setEnabled(false);
        ui->spbSeatColumn->setEnabled(false);
        ui->pbtModifyRowColumn->setEnabled(false);
    }
}

// Executes when ledSeatRow is clicked. Sets cursor position to 0 so user won't have problems typing. Since ledSeatRow has mask "a",
// typing will be difficult because user has to make sure he's in position 0 to input row. Otherwise mask won't allow him to type anything.
void TheaterAddModifyForm::on_ledSeatRow_pressed()
{
    ui->ledSeatRow->setCursorPosition(0);
}

// Modifies row and/or column of selected seat
void TheaterAddModifyForm::on_pbtModifyRowColumn_clicked()
{
    QString str = ui->ledSeatRow->text();
    char seatRow = str.toStdString().c_str()[0];

    // Get seats dragWidget has
    QList<SeatWidget*> labels = dragWidget->findChildren<SeatWidget*>();

    // Get index where selected seat is in "labels" list
    int seatIdx = dragWidget->getSeatIndex(labels, currentIdNumber);

    labels[seatIdx]->getSeat()->setSeatRow(seatRow);
    labels[seatIdx]->getSeat()->setSeatColumn(QString::number(ui->spbSeatColumn->value()));

    // If seat already exists in DB, label it as "modifiedSeat". Means we will perform an update command for this seat
    // when user wants to save progress. If this is a new seat, we won't do it since we will be using insert command.
    if(labels[seatIdx]->objectName() == "oldSeat")
        labels[seatIdx]->setObjectName("modifiedSeat");

    QString imagePath;
    if(labels[seatIdx]->getSeat()->getHandicap() == 0)
        imagePath = ":/Images/selected seat.png";
    else
        imagePath = ":/Images/selected handicap seat.png";

    dragWidget->addTextToImage(labels[seatIdx], imagePath, ui->ledSeatRow->text() + QString::number(ui->spbSeatColumn->value()));
}

void TheaterAddModifyForm::on_pbtAddHandicapSeat_clicked()
{
    addSeat(":/Images/available handicap seat.png", QPoint(10, 10));
}

// Removes seat from canvas.
void TheaterAddModifyForm::on_pbtRemoveSeat_clicked()
{
    QMessageBox::StandardButton reply;
    QString rowColumn = QChar(currentSeat.getSeatRow()) + currentSeat.getSeatColumn();
    reply = QMessageBox::question(this, "Remove seat", "Proceed with deletion of seat \"" + rowColumn + "\"?",
                                QMessageBox::Yes|QMessageBox::No);

    if (reply == QMessageBox::No) {
        return;
    }

    Seat removedSeat = dragWidget->removeSeat();

    // Append removed seat to removedSeatsList to keep track of removed seats for further delete from DB.
    if(removedSeat.getId() != 0)
        removedSeatsList.append(removedSeat);

    ui->pbtRemoveSeat->setEnabled(false);
}

void TheaterAddModifyForm::on_ledSeatRow_editingFinished()
{
    ui->ledSeatRow->setText(ui->ledSeatRow->text().toUpper());
}
