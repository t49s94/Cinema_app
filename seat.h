/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions related to seats.

*/

#ifndef SEAT_H
#define SEAT_H

#include "theater.h"

class Seat
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Seat();
    Seat(int id, Theater theater, char seatRow, QString seatColumn, bool handicap, int positionX, int positionY);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    Theater getTheater() const;
    void setTheater(const Theater &value);

    char getSeatRow() const;
    void setSeatRow(char value);

    QString getSeatColumn() const;
    void setSeatColumn(const QString &value);

    bool getHandicap() const;
    void setHandicap(bool value);

    int getPositionX() const;
    void setPositionX(int value);

    int getPositionY() const;
    void setPositionY(int value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    Theater theater;
    char seatRow;
    QString seatColumn;
    bool handicap;
    int positionX;
    int positionY;
};

#endif // SEAT_H
