#include "castposition.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains information about cast positions.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

CastPosition::CastPosition()
{
    setId(0);
    setPosition("");
}

CastPosition::CastPosition(int id, QString position)
{
    this->id = id;
    this->position = position;
}

CastPosition::CastPosition(CastPosition *position)
{
    this->id = position->getId();
    this->position = position->getPosition();
}

CastPosition::~CastPosition()
{

}

//------------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int CastPosition::getId() const
{
    return id;
}

void CastPosition::setId(int value)
{
    id = value;
}

QString CastPosition::getPosition() const
{
    return position;
}

void CastPosition::setPosition(const QString &value)
{
    position = value;
}


