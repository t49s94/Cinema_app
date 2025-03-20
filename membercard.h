/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information about member cards.

*/

#ifndef MEMBERCARD_H
#define MEMBERCARD_H

#include <qmainwindow.h>
#include <QDate>

class MemberCard
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    MemberCard();
    MemberCard(int id, int points, int currentYearVisits, QDate printDate);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    int getPoints() const;
    void setPoints(int value);

    int getCurrentYearVisits() const;
    void setCurrentYearVisits(int value);

    QDate getPrintDate() const;
    void setPrintDate(const QDate &value);
    void setPrintDate(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    int points;
    int currentYearVisits;
    QDate printDate;
};

#endif // MEMBERCARD_H
