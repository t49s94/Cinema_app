/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about theater types.

*/

#ifndef THEATERTYPE_H
#define THEATERTYPE_H

#include <qmainwindow.h>

class TheaterType
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    TheaterType();
    TheaterType(int id, QString name, double adultPrice, double kidPrice);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    double getAdultPrice() const;
    void setAdultPrice(double value);

    double getKidPrice() const;
    void setKidPrice(double value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
    double adultPrice;
    double kidPrice;
};

#endif // THEATERTYPE_H
