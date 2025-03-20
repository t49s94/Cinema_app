#include "coupon.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains information and functions about coupons.

*/

//------------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

Coupon::Coupon()
{
    setId(0);
    setPromotionType(PromotionType(0, "", 0, "", "X", CustomerType(0, "", 0, 0),
                                   TheaterType(0, "", 0, 0), PaymentType(0, ""), 0, 0, 0, 0, 0, 0, 0));
    setPrintDate(printDate);
    setExpirationDate(expirationDate);
    setUsed(used);
}

Coupon::Coupon(int id, PromotionType promotionType, QDate printDate, QDate expirationDate, bool used)
{
    setId(id);
    setPromotionType(promotionType);
    setPrintDate(printDate);
    setExpirationDate(expirationDate);
    setUsed(used);
}

//------------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Coupon::getId() const
{
    return id;
}

void Coupon::setId(int value)
{
    id = value;
}

PromotionType Coupon::getPromotionType() const
{
    return promotionType;
}

void Coupon::setPromotionType(const PromotionType &value)
{
    promotionType = value;
}

QDate Coupon::getPrintDate() const
{
    return printDate;
}

void Coupon::setPrintDate(const QDate &value)
{
    printDate = value;
}

void Coupon::setPrintDate(const QString &value)
{
    printDate = QDate::fromString(value, "yyyy-MM-dd");
}

QDate Coupon::getExpirationDate() const
{
    return expirationDate;
}

void Coupon::setExpirationDate(const QDate &value)
{
    expirationDate = value;
}

void Coupon::setExpirationDate(const QString &value)
{
    expirationDate = QDate::fromString(value, "yyyy-MM-dd");
}

bool Coupon::getUsed() const
{
    return used;
}

void Coupon::setUsed(bool value)
{
    used = value;
}
