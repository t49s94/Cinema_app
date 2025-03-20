#ifndef CUSTOMER_H
#define CUSTOMER_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains information and functions about customers.

*/

#include <qmainwindow.h>
#include <qcryptographichash.h>

#include "customertype.h"
#include "membercard.h"


class Customer
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    Customer();
    Customer(int id, QString firstName, QString middleName, QString lastName, QString username,
             QString password, CustomerType customerType, MemberCard memberCard, QDate dob, QChar gender,
             QString email, QString phone, QString streetAddress, QString city, QString stateCode,
             QString zipcode, QDate since);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getFirstName() const;
    void setFirstName(const QString &value);

    QString getMiddleName() const;
    void setMiddleName(const QString &value);

    QString getLastName() const;
    void setLastName(const QString &value);

    QString getUsername() const;
    void setUsername(const QString &value);

    QString getPassword() const;
    void setPassword(const QString &value);

    CustomerType getCustomerType() const;
    void setCustomerType(const CustomerType &value);

    MemberCard getMemberCard() const;
    void setMemberCard(const MemberCard &value);

    QDate getDob() const;
    void setDob(const QDate &value);
    void setDob(const QString &value);

    QString getPhone() const;
    void setPhone(const QString &value);

    QString getStreetAddress() const;
    void setStreetAddress(const QString &value);

    QString getCity() const;
    void setCity(const QString &value);

    QString getStateCode() const;
    void setStateCode(const QString &value);

    QString getZipcode() const;
    void setZipcode(const QString &value);

    QDate getSince() const;
    void setSince(const QDate &value);
    void setSince(const QString &value);

    QChar getGender() const;
    void setGender(const QChar &value);

    QString getEmail() const;
    void setEmail(const QString &value);

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Gets first, middle and last name concatenated.
    // @returns QString with full name.
    QString getFullName() const;

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString firstName;
    QString middleName;
    QString lastName;
    QString username;
    QString password;
    CustomerType customerType;
    MemberCard memberCard;
    QDate dob;
    QChar gender;
    QString email;
    QString phone;
    QString streetAddress;
    QString city;
    QString stateCode;
    QString zipcode;
    QDate since;
};

#endif // CUSTOMER_H
