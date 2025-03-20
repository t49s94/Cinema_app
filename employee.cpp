/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about employee.

*/

#include "employee.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Employee::Employee()
{
    setId(0);
    setFirstName("");
    setMiddleName("");
    setLastName("");
    setUsername("");
    setPassword("");
    setSsn("");
    setPosition(EmployeePosition());
    setDob(QDate());
    setGender(QChar());
    setEmail("");
    setPhone("");
    setStreetAddress("");
    setCity("");
    setStateCode("");
    setZipcode("");
    setSalary(0);
    setHiringDate(QDate());
}

Employee::Employee(int id, QString firstName, QString middleName, QString lastName, QString username, QString password,
                   QString ssn, EmployeePosition position, QDate dob, QChar gender, QString email, QString phone,
                   QString streetAddress, QString city, QString stateCode, QString zipcode, double salary, QDate hiringDate)
{
    setId(id);
    setFirstName(firstName);
    setMiddleName(middleName);
    setLastName(lastName);
    setUsername(username);
    setPassword(password);
    setSsn(ssn);
    setPosition(position);
    setDob(dob);
    setGender(gender);
    setEmail(email);
    setPhone(phone);
    setStreetAddress(streetAddress);
    setCity(city);
    setStateCode(stateCode);
    setZipcode(zipcode);
    setSalary(salary);
    setHiringDate(hiringDate);
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

QString Employee::getFullName() const
{
    if(middleName == "")
        return firstName + " " + lastName;
    else
        return firstName + " " + middleName + " " + lastName;
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Employee::getId() const
{
    return id;
}

void Employee::setId(int value)
{
    id = value;
}

QString Employee::getFirstName() const
{
    return firstName;
}

void Employee::setFirstName(const QString &value)
{
    firstName = value;
}

QString Employee::getMiddleName() const
{
    return middleName;
}

void Employee::setMiddleName(const QString &value)
{
    middleName = value;
}

QString Employee::getLastName() const
{
    return lastName;
}

void Employee::setLastName(const QString &value)
{
    lastName = value;
}

QString Employee::getUsername() const
{
    return username;
}

void Employee::setUsername(const QString &value)
{
    username = value;
}

QString Employee::getPassword() const
{
    return password;
}

void Employee::setPassword(const QString &value)
{
    password = QString("%1").arg(QString(QCryptographicHash::hash(value.toUtf8(), QCryptographicHash::Sha512).toHex()));;
}

QString Employee::getSsn() const
{
    return ssn;
}

void Employee::setSsn(const QString &value)
{
    ssn = value;
}

EmployeePosition Employee::getPosition() const
{
    return position;
}

void Employee::setPosition(const EmployeePosition &value)
{
    position = value;
}

QDate Employee::getDob() const
{
    return dob;
}

void Employee::setDob(const QDate &value)
{
    dob = value;
}

void Employee::setDob(const QString &value)
{
    dob = QDate::fromString(value, "yyyy-MM-dd");
}

QChar Employee::getGender() const
{
    return gender;
}

void Employee::setGender(const QChar &value)
{
    gender = value;
}

QString Employee::getEmail() const
{
    return email;
}

void Employee::setEmail(const QString &value)
{
    email = value;
}

QString Employee::getPhone() const
{
    return phone;
}

void Employee::setPhone(const QString &value)
{
    phone = value;
}

QString Employee::getStreetAddress() const
{
    return streetAddress;
}

void Employee::setStreetAddress(const QString &value)
{
    streetAddress = value;
}

QString Employee::getCity() const
{
    return city;
}

void Employee::setCity(const QString &value)
{
    city = value;
}

QString Employee::getStateCode() const
{
    return stateCode;
}

void Employee::setStateCode(const QString &value)
{
    stateCode = value;
}

QString Employee::getZipcode() const
{
    return zipcode;
}

void Employee::setZipcode(const QString &value)
{
    zipcode = value;
}

double Employee::getSalary() const
{
    return salary;
}

void Employee::setSalary(double value)
{
    salary = value;
}

QDate Employee::getHiringDate() const
{
    return hiringDate;
}

void Employee::setHiringDate(const QDate &value)
{
    hiringDate = value;
}

void Employee::setHiringDate(const QString &value)
{
    hiringDate = QDate::fromString(value, "yyyy-MM-dd");
}

