/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about promotions.

*/

#ifndef PROMOTION_H
#define PROMOTION_H

#include <QDate>

#include "promotiontype.h"

class Promotion
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Promotion();
    Promotion(int id, PromotionType promotionType, QDate startDate, QDate endDate);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    PromotionType getPromotionType() const;
    void setPromotionType(const PromotionType &value);

    QDate getStartDate() const;
    void setStartDate(const QDate &value);
    void setStartDate(const QString &value);

    QDate getEndDate() const;
    void setEndDate(const QDate &value);
    void setEndDate(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    PromotionType promotionType;
    QDate startDate;
    QDate endDate;

};

#endif // PROMOTION_H

