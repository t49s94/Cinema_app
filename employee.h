#ifndef EMPLOYEE_H
#define EMPLOYEE_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about employee.

*/

#include <qmainwindow.h>
#include <QDate>
#include <qcryptographichash.h>

#include "employeeposition.h"

class Employee
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Employee();
    Employee(int id, QString firstName, QString middleName, QString lastName, QString username,
             QString password, QString ssn,  EmployeePosition position, QDate dob, QChar gender,
             QString email, QString phone, QString streetAddress, QString city, QString stateCode,
             QString zipcode, double salary, QDate hiringDate);

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    QString getFullName() const;

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

    QString getSsn() const;
    void setSsn(const QString &value);

    EmployeePosition getPosition() const;
    void setPosition(const EmployeePosition &value);

    QDate getDob() const;
    void setDob(const QDate &value);
    void setDob(const QString &value);

    QChar getGender() const;
    void setGender(const QChar &value);

    QString getEmail() const;
    void setEmail(const QString &value);

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

    double getSalary() const;
    void setSalary(double value);

    QDate getHiringDate() const;
    void setHiringDate(const QDate &value);
    void setHiringDate(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString firstName;
    QString middleName;
    QString lastName;
    QString username;
    QString password;
    QString ssn;
    EmployeePosition position;
    QDate dob;
    QChar gender;
    QString email;
    QString phone;
    QString streetAddress;
    QString city;
    QString stateCode;
    QString zipcode;
    double salary;
    QDate hiringDate;

};

#endif // EMPLOYEE_H
