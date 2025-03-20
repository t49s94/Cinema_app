/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about employee position.

*/

#ifndef EMPLOYEEPOSITION_H
#define EMPLOYEEPOSITION_H

#include <qmainwindow.h>

class EmployeePosition
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    EmployeePosition();
    EmployeePosition(int id, QString name);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
};

#endif // EMPLOYEEPOSITION_H
