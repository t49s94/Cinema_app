/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about theater types.

*/

#include "theatertype.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

TheaterType::TheaterType()
{
    setId(0);
    setName("");
    setAdultPrice(0);
    setKidPrice(0);
}

TheaterType::TheaterType(int id, QString name, double adultPrice, double kidPrice)
{
    setId(id);
    setName(name);
    setAdultPrice(adultPrice);
    setKidPrice(kidPrice);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int TheaterType::getId() const
{
    return id;
}

void TheaterType::setId(int value)
{
    id = value;
}

QString TheaterType::getName() const
{
    return name;
}

void TheaterType::setName(const QString &value)
{
    name = value;
}

double TheaterType::getAdultPrice() const
{
    return adultPrice;
}

void TheaterType::setAdultPrice(double value)
{
    adultPrice = value;
}

double TheaterType::getKidPrice() const
{
    return kidPrice;
}

void TheaterType::setKidPrice(double value)
{
    kidPrice = value;
}
