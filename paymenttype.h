/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about payment types.

*/

#ifndef PAYMENTTYPE_H
#define PAYMENTTYPE_H

#include <QMainWindow>

class PaymentType
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    PaymentType();
    PaymentType(int id, QString name);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getName() const;
    void setName(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString name;
};

#endif // PAYMENTTYPE_H
