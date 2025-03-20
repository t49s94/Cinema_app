/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing promotion.

*/

#ifndef PROMOTIONADDMODIFYFORM_H
#define PROMOTIONADDMODIFYFORM_H

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "promotion.h"

namespace Ui {
class PromotionAddModifyForm;
}

class PromotionAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new promotion
    explicit PromotionAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList);
    // Constructor for updating existing promotion.
    explicit PromotionAddModifyForm(QWidget *parent, QList<QList<QString>> promotionTypesList, Promotion oldPromotion);
    ~PromotionAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initialize some features in form.
    // @param QList<QList<QString> > promotionTypesList. 2D List with Id and promotion type.
    void initializeForm(QList<QList<QString>> promotionTypesList);

    // Set the text of update query
    // @param QString* queryText. Hold Query text.
    // @return bool. True if at least 1 field will be updated
    bool setUpdateQueryText(QString* queryText);

    // Validates the fields in form.
    // @return bool. True if fields have correct data
    bool validateFields();

signals:

    // Says if pbtOk was clicked.
    // @param bool clicked. True if pbtOk was clicked.
    void is_pbtOK_clicked(bool clicked);

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::PromotionAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // 2D QList that contains Id and Promotion Type.
    QList<QList<QString>> promotionTypesList;

    // Original promotion data
    Promotion oldPromotion;
    // Updated promotion data
    Promotion newPromotion;
};

#endif // PROMOTIONADDMODIFYFORM_H
