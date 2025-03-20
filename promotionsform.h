/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays promotions stored in DB. Allows look up, add new, delete or update existing promotion.

*/

#ifndef PROMOTIONSFORM_H
#define PROMOTIONSFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QDate>

#include "login.h"
#include "promotion.h"
#include "promotionaddmodifyform.h"

namespace Ui {
class PromotionsForm;
}

class PromotionsForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit PromotionsForm(QWidget *parent = nullptr);
    ~PromotionsForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with information about promotions.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

    // Reset fields to default value in form.
    void resetFields();

private:
    Ui::PromotionsForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about promotions.
    QSqlQueryModel *model;

    // 2D QList that contains Id and Promotion Type
    QList<QList<QString>> promotionTypesList;

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    // Deletes selected promotions from table
    void on_pbtDelete_clicked();
    // Update selected promotions from table.
    void on_pbtModify_clicked();

    // Is executed when signal is_pbtOK_clicked from PromotionAddModifyForm is emitted.
    // @param bool clicked. True if push button "pbtOk" was clicked.
    void on_is_pbtOK_clicked(bool clicked);

    void on_chkStartDateFrom_stateChanged(int checked);
    void on_chkStartDateTo_stateChanged(int checked);
    void on_chkEndDateFrom_stateChanged(int checked);
    void on_chkEndDateTo_stateChanged(int checked);
};

#endif // PROMOTIONSFORM_H
