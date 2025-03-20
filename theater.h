/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains data and functions about theaters.

*/

#ifndef THEATER_H
#define THEATER_H

#include "theatertype.h"

class Theater
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Theater();
    Theater(int id, QString name, TheaterType theaterType);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    TheaterType getTheaterType() const;
    void setTheaterType(const TheaterType &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
    TheaterType theaterType;
};

#endif // THEATER_H
