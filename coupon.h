#ifndef COUPON_H
#define COUPON_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains information and functions about coupons.

*/

#include "promotiontype.h"
#include <QDate>

class Coupon
{
public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    Coupon();
    Coupon(int id, PromotionType promotionType, QDate printDate, QDate expirationDate, bool used);

    //------------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    PromotionType getPromotionType() const;
    void setPromotionType(const PromotionType &value);

    QDate getPrintDate() const;
    void setPrintDate(const QDate &value);
    void setPrintDate(const QString &value);

    QDate getExpirationDate() const;
    void setExpirationDate(const QDate &value);
    void setExpirationDate(const QString &value);

    bool getUsed() const;
    void setUsed(bool value);

private:

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    PromotionType promotionType;
    QDate printDate;
    QDate expirationDate;
    bool used;
};

#endif // COUPON_H
