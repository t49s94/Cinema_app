/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class creates a widget were items (seats) can be dragged and dropped around.

*/

#include <QtWidgets>

#include "dragwidget.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

//! [0]
DragWidget::DragWidget(QWidget *parent)
    : QFrame(parent)
{
    setMinimumSize(200, 200);
    //setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    setAcceptDrops(true);

    lastIdNumber = -1;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Add a SeatWidget to the class
// @param SeatWidget *label. Widget that's gonna be added
// @param QString path. Path of the image that SeatWidget will have.
// @param QPoint pos. Position where the SeatWidget is gonna be.
void DragWidget::addImage(SeatWidget *label, QString path, QPoint pos)
{
    QImage image(path);
    QPixmap pixmap(QPixmap::fromImage(image));
    // The image will be scaled
    label->setPixmap(pixmap.scaled(40,40,Qt::KeepAspectRatio));

    // label will have an unique Id number
    label->setIdNumber(++lastIdNumber);
    label->move(pos.x(), pos.y());
    label->show();
    // Deletes pointer when window is closed
    label->setAttribute(Qt::WA_DeleteOnClose);
}

// Add a SeatWidget to the class which will have a text on it
// @param SeatWidget *label. Widget that's gonna be added
// @param QString path. Path of the image that SeatWidget will have.
// @param QPoint pos. Position where the SeatWidget is gonna be.
// @param QString text. Text that will be drawn over the SeatWidget.
void DragWidget::addImage(SeatWidget *label, QString path, QPoint pos, QString text)
{
    QImage image(path);

    // Draws text over the SeatWidget
    QPainter p(&image);
    p.setPen(QPen(Qt::black));
    p.setFont(QFont("Times", 80, QFont::Bold));
    p.drawText(image.rect(), Qt::AlignCenter | Qt::AlignBottom, text);

    QPixmap pixmap(QPixmap::fromImage(image));
    // The image will be scaled
    label->setPixmap(pixmap.scaled(40,40,Qt::KeepAspectRatio));

    // label will have an unique Id number
    label->setIdNumber(++lastIdNumber);
    label->move(pos.x(), pos.y());
    label->show();
    // Deletes pointer when window is closed
    label->setAttribute(Qt::WA_DeleteOnClose);
}

// Draw a text over SeatWidget
// @param SeatWidget *label. Widget that will have the text drawn over.
// @param QString path. Path of the image that SeatWidget will have.
// @param QString text. Text that will be drawn over the SeatWidget.
void DragWidget::addTextToImage(SeatWidget *label, QString path, QString text)
{
    QImage image(path);

    // Draws text over the SeatWidget
    QPainter p(&image);
    p.setPen(QPen(Qt::black));
    p.setFont(QFont("Times", 80, QFont::Bold));
    p.drawText(image.rect(), Qt::AlignCenter | Qt::AlignBottom, text);

    QPixmap pixmap(QPixmap::fromImage(image));
    // The image will be scaled
    label->setPixmap(pixmap.scaled(40,40,Qt::KeepAspectRatio));
}

// Delete SeatWidget from draggable container
// @return Seat. Seat that was deleted.
Seat DragWidget::removeSeat()
{
    // Get SeatWidgets created.
    QList<SeatWidget*> labels = this->findChildren<SeatWidget*>();
    // Get index from SeatWidget that's gonna be deleted.
    int seatIdx = getSeatIndex(labels, currentIdNumber);
    Seat seat = *labels[seatIdx]->getSeat();
    labels[seatIdx]->close();

    return seat;
}

// Find a SeatWidget from QList.
// @param QList<SeatWidget*> labels.
// @param int idNumbers. ID from SeatWidget that wants to be found
// @return int. Index of SeatWidget found in QList.
int DragWidget::getSeatIndex(QList<SeatWidget*> labels, int idNumber)
{
    for(int i=0; i < labels.length(); i++)
        if(labels[i]->getIdNumber() == idNumber)
            return i;

    return -1;
}
//! [0]

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void DragWidget::dragEnterEvent(QDragEnterEvent *event)
{
    //-----------------------

    // Get SeatWidget that is in that position
    SeatWidget *child = static_cast<SeatWidget*>(childAt(event->pos()));
    // Return if there wasn't a SeatWidget in that position
    if (!child)
        return;

    // Get data from selected SeatWidget
    currentIdNumber = child->getIdNumber();
    currentSeat = child->getSeat();
    currentSeatName = child->objectName();

    //---------------------------

    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DragWidget::dragMoveEvent(QDragMoveEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void DragWidget::dropEvent(QDropEvent *event)
{
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        dataStream >> pixmap >> offset;

        // Create a copy of selected  SeatWidget from dragEnterEvent and add it in the new position to make the ilusion that it was
        // dragged and dropped.
        SeatWidget *newIcon = new SeatWidget(this, currentIdNumber, currentSeat, "");
        newIcon->setPixmap(pixmap);
        newIcon->move(event->pos() - offset);
        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);

        //-------------
        newIcon->setObjectName(currentSeatName);
        //-----------------------

        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();

        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

//! [1]
void DragWidget::mousePressEvent(QMouseEvent *event)
{
    // Get SeatWidget that is in that position
    SeatWidget *child = static_cast<SeatWidget*>(childAt(event->pos()));
    // Return if there wasn't a SeatWidget in that position
    if (!child)
        return;

    //QPixmap pixmap = child->pixmap(Qt::ReturnByValue);
    QPixmap pixmap = *child->pixmap();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos());
//! [1]

//! [2]
    QMimeData *mimeData = new QMimeData;
    mimeData->setData("application/x-dnditemdata", itemData);
//! [2]

//! [3]
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap(pixmap);
    drag->setHotSpot(event->pos() - child->pos());
//! [3]

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
        return;
    }

    // At this point there's a copy of selected SeatWidget which is "child". We set Id number to -1 to differentiate from original one.
    child->setIdNumber(-1);

    if(event->button() == Qt::LeftButton)
    {
        // Get list of SeatWidgets added to "this"
        QList<SeatWidget*> labels = this->findChildren<SeatWidget*>();

        int seatIdx = -1;
        int previousSeatIdx = -1;
        // Helps to know when both seat and previous seat were found. Break the loop when so.
        bool seatFound = false;
        bool previousSeatFound = false;
        // We get the index of the SeatWidgets that was selected first and second.
        for(int i=0; i < labels.length(); i++)
        {
            if(labels[i]->getIdNumber() == currentIdNumber)
            {
                seatIdx = i;
                seatFound = true;
            }
            if(labels[i]->getIdNumber() == previousIdNumber)
            {
                previousSeatIdx = i;
                previousSeatFound = true;
            }

            if(seatFound & previousSeatFound)
                break;
        }

        QString imagePath;

        // Seat was selected before so we turn it to available.
        //if(previousSeatFound && previousSeatIdx != seatIdx)
            if(previousSeatFound)
        {
            if(labels[previousSeatIdx]->getSeat()->getHandicap() == 0)
                imagePath = ":/Images/available seat.png";
            else
                imagePath = ":/Images/available handicap seat.png";

            addTextToImage(labels[previousSeatIdx], imagePath, QChar(labels[previousSeatIdx]->getSeat()->getSeatRow()) + labels[previousSeatIdx]->getSeat()->getSeatColumn());

        }

        // Seat is available so we turn it to selected.
        if(seatFound)
        {
            if(labels[seatIdx]->getSeat()->getHandicap() == 0)
                imagePath = ":/Images/selected seat.png";
            else
                imagePath = ":/Images/selected handicap seat.png";

            addTextToImage(labels[seatIdx], imagePath, QChar(labels[seatIdx]->getSeat()->getSeatRow()) + labels[seatIdx]->getSeat()->getSeatColumn());

            emit is_pressed("left", event->pos(), *labels[seatIdx]);
        }

    }

}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int DragWidget::getPreviousIdNumber() const
{
    return previousIdNumber;
}

void DragWidget::setPreviousIdNumber(int value)
{
    previousIdNumber = value;
}
