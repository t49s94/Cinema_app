#ifndef COUPONSFORM_H
#define COUPONSFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window displays coupons stored in database. Allows add, delete or modify existing coupon.

*/

#include <QMainWindow>

#include "login.h"
#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QDate>
#include "coupon.h"
#include "couponaddmodifyform.h"

namespace Ui {
class CouponsForm;
}

class CouponsForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    explicit CouponsForm(QWidget *parent = nullptr);
    ~CouponsForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with information about coupons.
    // @return bool. True if table was loaded succesfully.
    bool loadTable();

    // Resets fields in form
    void resetFields();

private:
    Ui::CouponsForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains informationn about coupons
    QSqlQueryModel *model;

    QList<QList<QString>> promotionTypesList;


private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    // Delete selected coupons
    void on_pbtDelete_clicked();
    // Modify selected coupon
    void on_pbtModify_clicked();

    void on_chkPrintDateFrom_stateChanged(int checked);
    void on_chkPrintDateTo_stateChanged(int checked);
    void on_chkExpirationDateFrom_stateChanged(int checked);
    void on_chkExpirationDateTo_stateChanged(int checked);

    // Executed when push button "pbtOK" from CouponAddModifyForm is clicked
    void on_is_pbtOK_clicked(bool clicked);
};

#endif // COUPONSFORM_H
