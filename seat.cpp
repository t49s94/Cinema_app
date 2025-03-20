/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions related to seats.

*/

#include "seat.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Seat::Seat()
{
    setId(0);
    setTheater(Theater());
    setSeatRow(' ');
    setSeatColumn("");
    setHandicap(false);
    setPositionX(0);
    setPositionY(0);
}

Seat::Seat(int id, Theater theater, char seatRow, QString seatColumn, bool handicap, int positionX, int positionY)
{
    setId(id);
    setTheater(theater);
    setSeatRow(seatRow);
    setSeatColumn(seatColumn);
    setHandicap(handicap);
    setPositionX(positionX);
    setPositionY(positionY);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Seat::getId() const
{
    return id;
}

void Seat::setId(int value)
{
    id = value;
}

Theater Seat::getTheater() const
{
    return theater;
}

void Seat::setTheater(const Theater &value)
{
    theater = value;
}

char Seat::getSeatRow() const
{
    return seatRow;
}

void Seat::setSeatRow(char value)
{
    seatRow = value;
}

QString Seat::getSeatColumn() const
{
    return seatColumn;
}

void Seat::setSeatColumn(const QString &value)
{
    seatColumn = value;
}

bool Seat::getHandicap() const
{
    return handicap;
}

void Seat::setHandicap(bool value)
{
    handicap = value;
}

int Seat::getPositionX() const
{
    return positionX;
}

void Seat::setPositionX(int value)
{
    positionX = value;
}

int Seat::getPositionY() const
{
    return positionY;
}

void Seat::setPositionY(int value)
{
    positionY = value;
}
