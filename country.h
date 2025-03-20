#ifndef COUNTRY_H
#define COUNTRY_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class cointains information about countries.

*/

#include <qmainwindow.h>

class Country
{
public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    Country();
    Country(int id, QString name);

    //------------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

private:

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
};

#endif // COUNTRY_H
