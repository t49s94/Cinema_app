/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains all the data used for creating a new boc office transaction.

*/

#include "boxofficetransaction.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors

BoxOfficeTransaction::BoxOfficeTransaction()
{
    setId(0);
    setMemberCard(MemberCard());
    setPaymentType(PaymentType());
    setCoupon(Coupon());
    setPromotion(Promotion());
    setCardNumber("0000000000000000");
    setPointsUsed(0);
    setNumberTickets(0);
    setTotal(0);
    setEmployee(Employee());
    setTransactionTime(QDateTime::currentDateTime());
}

BoxOfficeTransaction::BoxOfficeTransaction(int id, MemberCard memberCard, PaymentType paymentType, Coupon coupon, Promotion promotion,
                                           QString cardNumber, int pointsUsed, int numberTickets, double total, Employee employee, QDateTime transactionTime)
{
    setId(id);
    setMemberCard(memberCard);
    setPaymentType(paymentType);
    setCoupon(coupon);
    setPromotion(promotion);
    setCardNumber(cardNumber);
    setPointsUsed(pointsUsed);
    setNumberTickets(numberTickets);
    setTotal(total);
    setEmployee(employee);
    setTransactionTime(transactionTime);
}

//------------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int BoxOfficeTransaction::getId() const
{
    return id;
}

void BoxOfficeTransaction::setId(int value)
{
    id = value;
}

MemberCard BoxOfficeTransaction::getMemberCard() const
{
    return memberCard;
}

// Returns the address of the MemberCard object
MemberCard &BoxOfficeTransaction::getMemberCardAddress()
{
    return memberCard;
}

void BoxOfficeTransaction::setMemberCard(const MemberCard &value)
{
    memberCard = value;
}

PaymentType BoxOfficeTransaction::getPaymentType() const
{
    return paymentType;
}

// Returns the address of the PaymentType object
PaymentType &BoxOfficeTransaction::getPaymentTypeAddress()
{
    return paymentType;
}

void BoxOfficeTransaction::setPaymentType(const PaymentType &value)
{
    paymentType = value;
}

Coupon BoxOfficeTransaction::getCoupon() const
{
    return coupon;
}

// Returns the address of the Coupon object
Coupon &BoxOfficeTransaction::getCouponAddress()
{
    return coupon;
}

void BoxOfficeTransaction::setCoupon(const Coupon &value)
{
    coupon = value;
}

Promotion BoxOfficeTransaction::getPromotion() const
{
    return promotion;
}

// Returns the address of the Promotion object
Promotion &BoxOfficeTransaction::getPromotionAddress()
{
    return promotion;
}

void BoxOfficeTransaction::setPromotion(const Promotion &value)
{
    promotion = value;
}

QString BoxOfficeTransaction::getCardNumber() const
{
    return cardNumber;
}

void BoxOfficeTransaction::setCardNumber(const QString &value)
{
    cardNumber = value;
}

int BoxOfficeTransaction::getPointsUsed() const
{
    return pointsUsed;
}

void BoxOfficeTransaction::setPointsUsed(int value)
{
    pointsUsed = value;
}

int BoxOfficeTransaction::getNumberTickets() const
{
    return numberTickets;
}

void BoxOfficeTransaction::setNumberTickets(int value)
{
    numberTickets = value;
}

double BoxOfficeTransaction::getTotal() const
{
    return total;
}

void BoxOfficeTransaction::setTotal(double value)
{
    total = value;
}

Employee BoxOfficeTransaction::getEmployee() const
{
    return employee;
}

// Returns the address of the Employee object
Employee &BoxOfficeTransaction::getEmployeeAddress()
{
    return employee;
}

void BoxOfficeTransaction::setEmployee(const Employee &value)
{
    employee = value;
}

QDateTime BoxOfficeTransaction::getTransactionTime() const
{
    return transactionTime;
}

void BoxOfficeTransaction::setTransactionTime(const QDateTime &value)
{
    transactionTime = value;
}
