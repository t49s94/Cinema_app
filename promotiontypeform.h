/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays promotion types stored in DB. Allows look up, add new, delete or update existing promotion type.

*/

#ifndef PROMOTIONTYPEFORM_H
#define PROMOTIONTYPEFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <QListWidgetItem>
#include <qmessagebox.h>

#include "login.h"
#include "promotiontype.h"
#include "promotiontypeaddmodifyform.h"

namespace Ui {
class PromotionTypeForm;
}

class PromotionTypeForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit PromotionTypeForm(QWidget *parent = nullptr);
    ~PromotionTypeForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Populate combo boxes: cbxAgeCategory, cbxCustomerType, cbxTheaterType, cbxPaymentType.
    void populateComboBoxes();

    // Load table with information about promotion types.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

    // Resets fields to default value in form
    void resetFields();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    // Deletes selected promotion types from table
    void on_pbtDelete_clicked();
    // Update selected promotion types from table
    void on_pbtModify_clicked();

    // Executes when item in lwdDays is checked / unchecked. Highlights checked items.
    // @param QListWidgetItem *item. Item that was checked / unchecked.
    void highlightChecked(QListWidgetItem* item);

    // Executed when signal is_pbtOK_clicked is emitted. Says if push button "pbtOk" from PromotionTypeAddModifyForm was clicked.
    // @param bool clicked. True if pbtOk was clicked.
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::PromotionTypeForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about promotion types
    QSqlQueryModel *model;

    //2D List which containts Id and customer type
    QList<QList<QString>> customerTypesList;
    //2D List which containts Id and theater type
    QList<QList<QString>> theaterTypesList;
    //2D List which containts Id and payment type
    QList<QList<QString>> paymentTypesList;
};

#endif // PROMOTIONTYPEFORM_H
