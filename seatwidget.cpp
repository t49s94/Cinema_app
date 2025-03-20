/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class inherits from QLabel and acts like a seat in a theater.

*/

#include "seatwidget.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

SeatWidget::SeatWidget()
{

}

SeatWidget::SeatWidget(QWidget *parent) : QLabel()
{
    setParent(parent);
}

SeatWidget::SeatWidget(QWidget *parent, int id, Seat *seat, QString imagePath) : QLabel()
{
    initialize(parent, id, seat, imagePath);
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Initializes SeatWidget.
// @param QWidget *parent.
// @param int idNumber. Unique Id for Seat.
// @param Seat *seat. Information about seat.
// @param QString imagePath. Background image for Seat.
void SeatWidget::initialize(QWidget *parent, int idNumber, Seat *seat, QString imagePath)
{
    setParent(parent);
    setIdNumber(idNumber);
    setSeat(seat);
    setImagePath(imagePath);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int SeatWidget::getIdNumber() const
{
    return idNumber;
}

void SeatWidget::setIdNumber(int value)
{
    idNumber = value;
}

Seat *SeatWidget::getSeat() const
{
    return seat;
}

void SeatWidget::setSeat(Seat *value)
{
    seat = value;
}

QString SeatWidget::getImagePath() const
{
    return imagePath;
}

void SeatWidget::setImagePath(const QString &value)
{
    imagePath = value;
}
