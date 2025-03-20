#ifndef BOXOFFICETRANSACTION_H
#define BOXOFFICETRANSACTION_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains all the data used for creating a new boc office transaction.

*/

#include "membercard.h"
#include "paymenttype.h"
#include "coupon.h"
#include "promotion.h"
#include "employee.h"

class BoxOfficeTransaction
{
public:
    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors

    BoxOfficeTransaction();
    BoxOfficeTransaction(int id, MemberCard memberCard, PaymentType paymentType, Coupon coupon, Promotion promotion, QString cardNumber,
                         int pointsUsed, int numberTickets, double total, Employee employee, QDateTime transactionTime);

    //------------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    MemberCard getMemberCard() const;
    // Returns the address of the MemberCard object
    MemberCard& getMemberCardAddress();
    void setMemberCard(const MemberCard &value);

    PaymentType getPaymentType() const;
    // Returns the address of the PaymentType object
    PaymentType& getPaymentTypeAddress();
    void setPaymentType(const PaymentType &value);

    Coupon getCoupon() const;
    // Returns the address of the Coupon object
    Coupon& getCouponAddress();
    void setCoupon(const Coupon &value);

    Promotion getPromotion() const;
    // Returns the address of the Promotion object
    Promotion& getPromotionAddress();
    void setPromotion(const Promotion &value);

    QString getCardNumber() const;
    void setCardNumber(const QString &value);

    int getPointsUsed() const;
    void setPointsUsed(int value);

    int getNumberTickets() const;
    void setNumberTickets(int value);

    double getTotal() const;
    void setTotal(double value);

    Employee getEmployee() const;
    // Returns the address of the Employee object
    Employee& getEmployeeAddress();
    void setEmployee(const Employee &value);

    QDateTime getTransactionTime() const;
    void setTransactionTime(const QDateTime &value);

private:

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    MemberCard memberCard;
    PaymentType paymentType;
    Coupon coupon;
    Promotion promotion;
    QString cardNumber;
    int pointsUsed;
    int numberTickets;
    double total;
    Employee employee;
    QDateTime transactionTime;
};

#endif // BOXOFFICETRANSACTION_H
