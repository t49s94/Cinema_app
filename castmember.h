#ifndef CASTMEMBER_H
#define CASTMEMBER_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains all the data related to a cast member.

*/

#include <qmainwindow.h>
#include <QDate>

#include "castposition.h"
#include "country.h"

class CastMember
{
public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    CastMember();
    CastMember(int id, QString firstName, QString middleName, QString lastName, QDate dob,
               Country country, QString gender, QList<CastPosition> positionsList);
    ~CastMember();

    //------------------------------------------------------------------------------------------------------------------------------
    // Functions

    void appendPosition(CastPosition position);

    //------------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    CastPosition getPositionAt(int index);
    QString getFullName();

    int getId() const;
    void setId(int value);

    QString getFirstName() const;
    void setFirstName(const QString &value);

    QString getMiddleName() const;
    void setMiddleName(const QString &value);

    QString getLastName() const;
    void setLastName(const QString &value);

    QDate getDOB() const;
    void setDOB(const QDate &value);
    void setDOB(const QString &value);

    Country getCountry() const;
    void setCountry(const Country &value);

    QString getGender() const;
    void setGender(const QString &value);

    QList<CastPosition> getPositionsList() const;
    void setPositionsList(const QList<CastPosition> &value);

private:

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString firstName;
    QString middleName;
    QString lastName;
    QDate dob;
    Country country;
    QString gender;
    QList<CastPosition> positionsList;

};

#endif // CASTMEMBER_H
