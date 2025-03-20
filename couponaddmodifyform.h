#ifndef COUPONADDMODIFYFORM_H
#define COUPONADDMODIFYFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window allows add or update existing coupon.

*/

#include <QDialog>
#include <qmessagebox.h>

#include "login.h"
#include "coupon.h"


namespace Ui {
class CouponAddModifyForm;
}

class CouponAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    // Constructor for adding new coupon
    explicit CouponAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList);
    // Constructor for updating existing coupon
    explicit CouponAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList, Coupon oldCoupon);
    ~CouponAddModifyForm();

    //------------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initialize some features of this form.
    // @param QList<QList<QString>> promotionTypesList.
    void initializeForm(QList<QList<QString>> promotionTypesList);

    // Creates query text for update.
    // @param QString* queryText. QString that contains update query text
    // @return bool. False if there were no changes made.
    bool setUpdateQueryText(QString* queryText);

signals:
    // @param bool clicked. True if push button "pbtOK" is clicked.
    void is_pbtOK_clicked(bool clicked);

private slots:
    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::CouponAddModifyForm *ui;

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;
    QList<QList<QString>> promotionTypesList;
    // Coupon to be modified
    Coupon oldCoupon;
    // Coupon modified
    Coupon newCoupon;
};

#endif // COUPONADDMODIFYFORM_H
