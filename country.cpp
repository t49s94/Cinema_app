#include "country.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class cointains information about countries.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

Country::Country()
{
    id = 0;
    name = "";
}

Country::Country(int id, QString name)
{
    this->id = id;
    this->name = name;
}

//------------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Country::getId() const
{
    return id;
}

void Country::setId(int value)
{
    id = value;
}

QString Country::getName() const
{
    return name;
}

void Country::setName(const QString &value)
{
    name = value;
}
