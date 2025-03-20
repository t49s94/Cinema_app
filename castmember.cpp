#include "castmember.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains all the data related to a cast member.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

CastMember::CastMember()
{
    setId(0);
    setFirstName("");
    setMiddleName("");
    setLastName("");
    setDOB(QDate::currentDate());
    setCountry(Country());
    setGender("X");
    setPositionsList(QList<CastPosition>());
}

CastMember::CastMember(int id, QString firstName, QString middleName, QString lastName, QDate dob, Country country,
                       QString gender, QList<CastPosition> positionsList)
{
    this->id = id;
    this->firstName = firstName;
    this->middleName = middleName;
    this->lastName = lastName;
    this->dob = dob;
    this->country = country;
    this->gender = gender;
    this->positionsList = positionsList;
}

CastMember::~CastMember()
{

}

//------------------------------------------------------------------------------------------------------------------------------
// Functions

void CastMember::appendPosition(CastPosition position)
{
    this->positionsList.append(position);
}

//------------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

CastPosition CastMember::getPositionAt(int index)
{
    return this->positionsList.at(index);
}

QString CastMember::getFullName()
{
    if(middleName != "")
        return firstName + " " + middleName + " " + lastName;
    else
        return firstName + " " + lastName;
}

int CastMember::getId() const
{
    return id;
}

void CastMember::setId(int value)
{
    id = value;
}

QString CastMember::getFirstName() const
{
    return firstName;
}

void CastMember::setFirstName(const QString &value)
{
    firstName = value;
}

QString CastMember::getMiddleName() const
{
    return middleName;
}

void CastMember::setMiddleName(const QString &value)
{
    middleName = value;
}

QString CastMember::getLastName() const
{
    return lastName;
}

void CastMember::setLastName(const QString &value)
{
    lastName = value;
}

void CastMember::setDOB(const QString &value)
{
    this->dob = QDate::fromString(value, "yyyy-MM-dd");
}

QDate CastMember::getDOB() const
{
    return dob;
}

void CastMember::setDOB(const QDate &value)
{
    dob = value;
}

QString CastMember::getGender() const
{
    return gender;
}

void CastMember::setGender(const QString &value)
{
    gender = value;
}

QList<CastPosition> CastMember::getPositionsList() const
{
    return positionsList;
}

void CastMember::setPositionsList(const QList<CastPosition> &value)
{
    positionsList = value;
}

Country CastMember::getCountry() const
{
    return country;
}

void CastMember::setCountry(const Country &value)
{
    country = value;
}
