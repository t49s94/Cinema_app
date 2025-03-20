/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class creates a widget that acts like a theater where there are seats which employee can select for people who want to buy tickets

*/

#include "theaterwidget.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

TheaterWidget::TheaterWidget(QWidget *parent, int numberPeople)
    : QFrame(parent)
{
    setMinimumSize(200, 200);
    setAcceptDrops(true);
    lastIdNumber = -1;
    this->numberPeople = numberPeople;

    selectedSeats = new QList<Seat>();
    selectedSeatIdNumbers = new QList<int>();
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Add a SeatWidget to the class
// @param SeatWidget *label. Widget that's gonna be added
// @param QString path. Path of the image that SeatWidget will have.
// @param QPoint pos. Position where the SeatWidget will be.
// @param QString text. Row and Column of seat.
void TheaterWidget::addImage(SeatWidget *label, QString path, QPoint pos, QString text)
{
    QImage image(path);

    QPainter p(&image);
    p.setPen(QPen(Qt::black));
    p.setFont(QFont("Times", 80, QFont::Bold));
    p.drawText(image.rect(), Qt::AlignCenter | Qt::AlignBottom, text);

    label->setIdNumber(++lastIdNumber);
    label->setImagePath(path);

    QPixmap pixmap(QPixmap::fromImage(image));
    label->setPixmap(pixmap.scaled(40,40,Qt::KeepAspectRatio));
    label->move(pos.x(), pos.y());
    label->show();
    label->setAttribute(Qt::WA_DeleteOnClose);
}

// Draw a text over SeatWidget
// @param SeatWidget *label. Widget that will have the text drawn over.
// @param QString path. Path of the image that SeatWidget will have.
// @param QString text. Text that will be drawn over the SeatWidget.
void TheaterWidget::addTextToImage(SeatWidget *label, QString path, QString text)
{
    QImage image(path);

    QPainter p(&image);
    p.setPen(QPen(Qt::black));
    p.setFont(QFont("Times", 80, QFont::Bold));
    p.drawText(image.rect(), Qt::AlignCenter | Qt::AlignBottom, text);

    label->setImagePath(path);

    QPixmap pixmap(QPixmap::fromImage(image));
    label->setPixmap(pixmap.scaled(40,40,Qt::KeepAspectRatio));
    //label->move(label->pos());
    //label->show();
    //label->setAttribute(Qt::WA_DeleteOnClose);
}

// Find a SeatWidget from QList.
// @param QList<SeatWidget*> labels.
// @param int idNumbers. ID from SeatWidget that wants to be found
// @return int. Index of SeatWidget found in QList.
int TheaterWidget::getSeatIndex(QList<SeatWidget *> labels, int idNumber)
{
    for(int i=0; i < labels.length(); i++)
    {
        if(labels[i]->getIdNumber() == idNumber)
        {
            return i;
        }
    }

    return -1;
}
//! [0]

//----------------------------------------------------------------------------------------------------------------------------
// Slots

//! [1]
void TheaterWidget::mousePressEvent(QMouseEvent *event)
{
    // Get SeatWidget that is in that position
    SeatWidget *child = static_cast<SeatWidget*>(childAt(event->pos()));
    // Return if there wasn't a SeatWidget in that position
    if (!child)
        return;

    currentIdNumber = child->getIdNumber();
    //child->setIdNumber(-1);

    if(event->button() == Qt::LeftButton)
    {
        // If seat is available, change state to "selected"
        if((child->getImagePath() == ":/Images/available seat.png" || child->getImagePath() == ":/Images/available handicap seat.png")
                && selectedSeatIdNumbers->length() < numberPeople)
        {
            // Get list of SeatWidgets added to "this"
            QList<SeatWidget*> labels = this->findChildren<SeatWidget*>();

            int seatIdx = getSeatIndex(labels, currentIdNumber);

            QString imagePath;
    \
            if(labels[seatIdx]->getSeat()->getHandicap() == 0)
                imagePath = ":/Images/selected seat.png";
            else
                imagePath = ":/Images/selected handicap seat.png";


            QString rowColumn = QChar(labels[seatIdx]->getSeat()->getSeatRow()) + labels[seatIdx]->getSeat()->getSeatColumn();

            addTextToImage(labels[seatIdx], imagePath, rowColumn);

            selectedSeatIdNumbers->append(currentIdNumber);
            selectedSeats->append(*labels[seatIdx]->getSeat());

            emit is_pressed("left", event->pos(), *labels[seatIdx]);

        }
        else
            //If seat was selected already, change to "available"
            if(child->getImagePath() == ":/Images/selected seat.png" || child->getImagePath() == ":/Images/selected handicap seat.png")
            {
                QList<SeatWidget*> labels = this->findChildren<SeatWidget*>();

                int seatIdx = getSeatIndex(labels, currentIdNumber);

                QString imagePath;
        \
                if(labels[seatIdx]->getSeat()->getHandicap() == 0)
                    imagePath = ":/Images/available seat.png";
                else
                    imagePath = ":/Images/available handicap seat.png";

                QString rowColumn = QChar(labels[seatIdx]->getSeat()->getSeatRow()) + labels[seatIdx]->getSeat()->getSeatColumn();

                addTextToImage(labels[seatIdx], imagePath, rowColumn);

                int selectedSeatIdx = selectedSeatIdNumbers->indexOf(currentIdNumber);
                selectedSeatIdNumbers->takeAt(selectedSeatIdx);
                selectedSeats->takeAt(selectedSeatIdx);

                emit is_pressed("left", event->pos(), *labels[seatIdx]);
            }


    }


}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

QList<Seat> *TheaterWidget::getSelectedSeats() const
{
    return selectedSeats;
}

void TheaterWidget::setSelectedSeats(QList<Seat> *value)
{
    selectedSeats = value;
}

QList<int> *TheaterWidget::getSelectedSeatIdNumbers() const
{
    return selectedSeatIdNumbers;
}

void TheaterWidget::setSelectedSeatIdNumbers(QList<int> *value)
{
    selectedSeatIdNumbers = value;
}

int TheaterWidget::getNumberPeople() const
{
    return numberPeople;
}

void TheaterWidget::setNumberPeople(int value)
{
    numberPeople = value;
}

