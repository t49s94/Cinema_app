/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information about member cards.

*/

#include "membercard.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MemberCard::MemberCard()
{
    setId(0);
    setPoints(0);
    setCurrentYearVisits(0);
    setPrintDate(QDate());
}

MemberCard::MemberCard(int id, int points, int currentYearVisits, QDate printDate)
{   
    setId(id);
    setPoints(points);
    setCurrentYearVisits(currentYearVisits);
    setPrintDate(printDate);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int MemberCard::getId() const
{
    return id;
}

void MemberCard::setId(int value)
{
    id = value;
}

int MemberCard::getPoints() const
{
    return points;
}

void MemberCard::setPoints(int value)
{
    points = value;
}

int MemberCard::getCurrentYearVisits() const
{
    return currentYearVisits;
}

void MemberCard::setCurrentYearVisits(int value)
{
    currentYearVisits = value;
}

QDate MemberCard::getPrintDate() const
{
    return printDate;
}

void MemberCard::setPrintDate(const QDate &value)
{
    printDate = value;
}

void MemberCard::setPrintDate(const QString &value)
{
    printDate = QDate::fromString(value, "yyyy-MM-dd");
}
