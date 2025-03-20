#ifndef CUSTOMERTYPE_H
#define CUSTOMERTYPE_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information about customer types.

*/

#include <qmainwindow.h>

class CustomerType
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    CustomerType();
    CustomerType(int id, QString name, int cumulation, int visitsRequired);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    int getCumulation() const;
    void setCumulation(int value);

    int getVisitsRequired() const;
    void setVisitsRequired(int value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
    // percentage of the sale returned to the customer in form of points
    int cumulation;
    // visits required per year to get to this level
    int visitsRequired;

};

#endif // CUSTOMERTYPE_H
