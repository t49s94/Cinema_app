#include "customertype.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

CustomerType::CustomerType()
{
    setId(0);
    setName("");
    setCumulation(0);
    setVisitsRequired(0);
}

CustomerType::CustomerType(int id, QString name, int cumulation, int visitsRequired)
{
    this->id = id;
    this->name = name;
    this->cumulation = cumulation;
    this->visitsRequired = visitsRequired;
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int CustomerType::getId() const
{
    return id;
}

void CustomerType::setId(int value)
{
    id = value;
}

QString CustomerType::getName() const
{
    return name;
}

void CustomerType::setName(const QString &value)
{
    name = value;
}

int CustomerType::getCumulation() const
{
    return cumulation;
}

void CustomerType::setCumulation(int value)
{
    cumulation = value;
}

int CustomerType::getVisitsRequired() const
{
    return visitsRequired;
}

void CustomerType::setVisitsRequired(int value)
{
    visitsRequired = value;
}
