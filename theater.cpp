/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains data and functions about theaters.

*/

#include "theater.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Theater::Theater()
{
    setId(0);
    setName("");
    setTheaterType(TheaterType());
}

Theater::Theater(int id, QString name, TheaterType theaterType)
{
    setId(id);
    setName(name);
    setTheaterType(theaterType);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Theater::getId() const
{
    return id;
}

void Theater::setId(int value)
{
    id = value;
}

QString Theater::getName() const
{
    return name;
}

void Theater::setName(const QString &value)
{
    name = value;
}

TheaterType Theater::getTheaterType() const
{
    return theaterType;
}

void Theater::setTheaterType(const TheaterType &value)
{
    theaterType = value;
}
