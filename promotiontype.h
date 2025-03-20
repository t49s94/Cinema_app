/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about promotion type.

*/

#ifndef PROMOTIONTYPE_H
#define PROMOTIONTYPE_H

#include <QMainWindow>

#include "customertype.h"
#include "theatertype.h"
#include "paymenttype.h"

class PromotionType
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    PromotionType();
    PromotionType(int id, QString name, int discount, QString description, QString ageCategory,
                  CustomerType customerType, TheaterType theaterType, PaymentType paymentType, bool monday,
                  bool tuesday, bool wednesday, bool thursday, bool friday, bool saturday, bool sunday);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

    int getDiscount() const;
    void setDiscount(int value);

    QString getDescription() const;
    void setDescription(const QString &value);

    QString getAgeCategory() const;
    void setAgeCategory(const QString &value);

    CustomerType getCustomerType() const;
    void setCustomerType(const CustomerType &value);

    TheaterType getTheaterType() const;
    void setTheaterType(const TheaterType &value);

    PaymentType getPaymentType() const;
    void setPaymentType(const PaymentType &value);

    bool getMonday() const;
    void setMonday(bool value);

    bool getTuesday() const;
    void setTuesday(bool value);

    bool getWednesday() const;
    void setWednesday(bool value);

    bool getThursday() const;
    void setThursday(bool value);

    bool getFriday() const;
    void setFriday(bool value);

    bool getSaturday() const;
    void setSaturday(bool value);

    bool getSunday() const;
    void setSunday(bool value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
    int discount;
    QString description;
    QString ageCategory;
    CustomerType customerType;
    TheaterType theaterType;
    PaymentType paymentType;
    bool monday;
    bool tuesday;
    bool wednesday;
    bool thursday;
    bool friday;
    bool saturday;
    bool sunday;
};

#endif // PROMOTIONTYPE_H
