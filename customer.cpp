#include "customer.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains information and functions about customers.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructors

Customer::Customer()
{
    setId(0);
    setFirstName("");
    setMiddleName("");
    setLastName("");
    setUsername("");
    setPassword("");
    setCustomerType(CustomerType());
    setMemberCard(MemberCard());
    setDob("");
    setGender('M');
    setEmail("");
    setPhone("");
    setStreetAddress("");
    setCity("");
    setStateCode("");
    setZipcode("");
    setSince("");
}

Customer::Customer(int id, QString firstName, QString middleName, QString lastName, QString username,
                   QString password, CustomerType customerType, MemberCard memberCard, QDate dob, QChar gender,
                   QString email, QString phone, QString streetAddress, QString city, QString stateCode,
                   QString zipcode, QDate since)
{
    setId(id);
    setFirstName(firstName);
    setMiddleName(middleName);
    setLastName(lastName);
    setUsername(username);
    setPassword(password);
    setCustomerType(customerType);
    setMemberCard(memberCard);
    setDob(dob);
    setGender(gender);
    setEmail(email);
    setPhone(phone);
    setStreetAddress(streetAddress);
    setCity(city);
    setStateCode(stateCode);
    setZipcode(zipcode);
    setSince(since);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Customer::getId() const
{
    return id;
}

void Customer::setId(int value)
{
    id = value;
}

QString Customer::getFirstName() const
{
    return firstName;
}

void Customer::setFirstName(const QString &value)
{
    firstName = value;
}

QString Customer::getMiddleName() const
{
    return middleName;
}

void Customer::setMiddleName(const QString &value)
{
    middleName = value;
}

QString Customer::getLastName() const
{
    return lastName;
}

void Customer::setLastName(const QString &value)
{
    lastName = value;
}

QString Customer::getUsername() const
{
    return username;
}

void Customer::setUsername(const QString &value)
{
    username = value;
}

QString Customer::getPassword() const
{
    return password;
}

void Customer::setPassword(const QString &value)
{
    // Encript with Sha512 algorithm
    password = QString("%1").arg(QString(QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha512).toHex()));;
}

CustomerType Customer::getCustomerType() const
{
    return customerType;
}

void Customer::setCustomerType(const CustomerType &value)
{
    customerType = value;
}

MemberCard Customer::getMemberCard() const
{
    return memberCard;
}

void Customer::setMemberCard(const MemberCard &value)
{
    memberCard = value;
}

QDate Customer::getDob() const
{
    return dob;
}

void Customer::setDob(const QDate &value)
{
    dob = value;
}

void Customer::setDob(const QString &value)
{
    dob = QDate::fromString(value, "yyyy-MM-dd");
}

QString Customer::getPhone() const
{
    return phone;
}

void Customer::setPhone(const QString &value)
{
    phone = value;
}

QString Customer::getStreetAddress() const
{
    return streetAddress;
}

void Customer::setStreetAddress(const QString &value)
{
    streetAddress = value;
}

QString Customer::getCity() const
{
    return city;
}

void Customer::setCity(const QString &value)
{
    city = value;
}

QString Customer::getStateCode() const
{
    return stateCode;
}

void Customer::setStateCode(const QString &value)
{
    stateCode = value;
}

QString Customer::getZipcode() const
{
    return zipcode;
}

void Customer::setZipcode(const QString &value)
{
    zipcode = value;
}

QDate Customer::getSince() const
{
    return since;
}

void Customer::setSince(const QDate &value)
{
    since = value;
}

void Customer::setSince(const QString &value)
{
    since = QDate::fromString(value, "yyyy-MM-dd");
}

QString Customer::getEmail() const
{
    return email;
}

void Customer::setEmail(const QString &value)
{
    email = value;
}

QChar Customer::getGender() const
{
    return gender;
}

void Customer::setGender(const QChar &value)
{
    gender = value;
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

// Gets first, middle and last name concatenated.
// @returns QString with full name.
QString Customer::getFullName() const
{
    if(middleName == "")
        return firstName + " " + lastName;
    else
        return firstName + " " + middleName + " " + lastName;
}
