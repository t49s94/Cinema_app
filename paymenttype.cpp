/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about payment types.

*/

#include "paymenttype.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

PaymentType::PaymentType()
{
    setId(0);
    setName("");
}

PaymentType::PaymentType(int id, QString name)
{
    setId(id);
    setName(name);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int PaymentType::getId() const
{
    return id;
}

void PaymentType::setId(int value)
{
    id = value;
}

QString PaymentType::getName() const
{
    return name;
}

void PaymentType::setName(const QString &value)
{
    name = value;
}
