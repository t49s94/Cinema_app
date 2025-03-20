/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about promotion type.

*/

#include "promotiontype.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

PromotionType::PromotionType()
{
    setId(0);
    setName("");
    setDiscount(0);
    setDescription("");
    setAgeCategory("");
    setCustomerType(CustomerType(0, "", 0, 0));
    setTheaterType(TheaterType(0, "", 0, 0));
    setPaymentType(PaymentType(0, ""));
    setMonday(0);
    setTuesday(0);
    setWednesday(0);
    setThursday(0);
    setFriday(0);
    setSaturday(0);
    setSunday(0);
}

PromotionType::PromotionType(int id, QString name, int discount, QString description, QString ageCategory,
                             CustomerType customerType, TheaterType theaterType, PaymentType paymentType, bool monday,
                             bool tuesday, bool wednesday, bool thursday, bool friday, bool saturday, bool sunday)
{
    setId(id);
    setName(name);
    setDiscount(discount);
    setDescription(description);
    setAgeCategory(ageCategory);
    setCustomerType(customerType);
    setTheaterType(theaterType);
    setPaymentType(paymentType);
    setMonday(monday);
    setTuesday(tuesday);
    setWednesday(wednesday);
    setThursday(thursday);
    setFriday(friday);
    setSaturday(saturday);
    setSunday(sunday);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int PromotionType::getId() const
{
    return id;
}

void PromotionType::setId(int value)
{
    id = value;
}

QString PromotionType::getName() const
{
    return name;
}

void PromotionType::setName(const QString &value)
{
    name = value;
}

int PromotionType::getDiscount() const
{
    return discount;
}

void PromotionType::setDiscount(int value)
{
    discount = value;
}

QString PromotionType::getDescription() const
{
    return description;
}

void PromotionType::setDescription(const QString &value)
{
    description = value;
}

QString PromotionType::getAgeCategory() const
{
    return ageCategory;
}

void PromotionType::setAgeCategory(const QString &value)
{
    ageCategory = value;
}

CustomerType PromotionType::getCustomerType() const
{
    return customerType;
}

void PromotionType::setCustomerType(const CustomerType &value)
{
    customerType = value;
}

TheaterType PromotionType::getTheaterType() const
{
    return theaterType;
}

void PromotionType::setTheaterType(const TheaterType &value)
{
    theaterType = value;
}

PaymentType PromotionType::getPaymentType() const
{
    return paymentType;
}

void PromotionType::setPaymentType(const PaymentType &value)
{
    paymentType = value;
}

bool PromotionType::getMonday() const
{
    return monday;
}

void PromotionType::setMonday(bool value)
{
    monday = value;
}

bool PromotionType::getTuesday() const
{
    return tuesday;
}

void PromotionType::setTuesday(bool value)
{
    tuesday = value;
}

bool PromotionType::getWednesday() const
{
    return wednesday;
}

void PromotionType::setWednesday(bool value)
{
    wednesday = value;
}

bool PromotionType::getThursday() const
{
    return thursday;
}

void PromotionType::setThursday(bool value)
{
    thursday = value;
}

bool PromotionType::getFriday() const
{
    return friday;
}

void PromotionType::setFriday(bool value)
{
    friday = value;
}

bool PromotionType::getSaturday() const
{
    return saturday;
}

void PromotionType::setSaturday(bool value)
{
    saturday = value;
}

bool PromotionType::getSunday() const
{
    return sunday;
}

void PromotionType::setSunday(bool value)
{
    sunday = value;
}
