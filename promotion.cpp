/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about promotions.

*/

#include "promotion.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Promotion::Promotion()
{
    setId(0);
    setPromotionType(PromotionType(0, "", 0, "", "X", CustomerType(0, "", 0, 0),
                                   TheaterType(0, "", 0, 0), PaymentType(0, ""), 0, 0, 0, 0, 0, 0, 0));

    setStartDate(QDate::currentDate());
    QDate endDateAux = QDate::currentDate();
    endDateAux.addDays(1);
    setEndDate(endDateAux);
}

Promotion::Promotion(int id, PromotionType promotionType, QDate startDate, QDate endDate)
{
    setId(id);
    setPromotionType(promotionType);
    setStartDate(startDate);
    setEndDate(endDate);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Promotion::getId() const
{
    return id;
}

void Promotion::setId(int value)
{
    id = value;
}

PromotionType Promotion::getPromotionType() const
{
    return promotionType;
}

void Promotion::setPromotionType(const PromotionType &value)
{
    promotionType = value;
}

QDate Promotion::getStartDate() const
{
    return startDate;
}

void Promotion::setStartDate(const QDate &value)
{
    startDate = value;
}

void Promotion::setStartDate(const QString &value)
{
    startDate = QDate::fromString(value, "yyyy-MM-dd");
}

QDate Promotion::getEndDate() const
{
    return endDate;
}

void Promotion::setEndDate(const QDate &value)
{
    endDate = value;
}

void Promotion::setEndDate(const QString &value)
{
    endDate = QDate::fromString(value, "yyyy-MM-dd");
}
