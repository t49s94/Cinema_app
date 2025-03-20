/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about employee position.

*/

#include "employeeposition.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

EmployeePosition::EmployeePosition()
{
    setId(0);
    setName("");
}

EmployeePosition::EmployeePosition(int id, QString name)
{
    setId(id);
    setName(name);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int EmployeePosition::getId() const
{
    return id;
}

void EmployeePosition::setId(int value)
{
    id = value;
}

QString EmployeePosition::getName() const
{
    return name;
}

void EmployeePosition::setName(const QString &value)
{
    name = value;
}
