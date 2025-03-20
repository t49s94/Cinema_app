/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows add new or update existing promotion type.

*/

#ifndef PROMOTIONTYPEADDMODIFYFORM_H
#define PROMOTIONTYPEADDMODIFYFORM_H

#include <QDialog>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QListWidgetItem>

#include "promotiontype.h"
#include "login.h"

namespace Ui {
class PromotionTypeAddModifyForm;
}

class PromotionTypeAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new promotion type
    explicit PromotionTypeAddModifyForm(QWidget *parent, QList<QList<QString>> customerTypesList, QList<QList<QString>> theaterTypesList,
                                        QList<QList<QString>> paymentTypesList);
    // Constructor for updating existing promotion type
    explicit PromotionTypeAddModifyForm(QWidget *parent, QList<QList<QString>> customerTypesList, QList<QList<QString>> theaterTypesList,
                                        QList<QList<QString>> paymentTypesList, PromotionType oldPromotionType);
    ~PromotionTypeAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Capitalize QString text.
    // @param const QString &str. QString that's gonna be capitalized.
    // @return QString. Capitalized QString.
    QString capitalize(const QString &str);

    // Initialize some features of form.
    // @param QList<QList<QString>> customerTypesList. 2D QList that contains Id and customer type.
    // @param QList<QList<QString>> theaterTypesList. 2D QList that contains Id and theater type.
    // @param QList<QList<QString>> paymentTypesList. 2D QList that contains Id and payment type.
    void initializeForm(QList<QList<QString>> customerTypesList, QList<QList<QString>> theaterTypesList, QList<QList<QString>> paymentTypesList);

    // Checks that required fields were filled out.
    // @return bool. True if all required fields were filled out.
    bool areFieldsNotBlank();

    // Sets text of update query text.
    // @param QString *queryText. Holds text of update query.
    // @return bool. True if at least 1 fields wants to be updated.
    bool setUpdateQueryText(QString* queryText);

signals:

    // Says if pbtOk was clicked.
    // @param bool clicked. True if pbtOk was clicked.
    void is_pbtOK_clicked(bool clicked);

private slots:

    // Is executed when QListWidgetItem is checked or unchecked. Highlights checked items.
    // @param QListWidgetItem *item. Item that was checked / unchecked.
    void highlightChecked(QListWidgetItem* item);

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::PromotionTypeAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // 2D QList that contains Id and customer type.
    QList<QList<QString>> customerTypesList;
    // 2D QList that contains Id and theater type.
    QList<QList<QString>> theaterTypesList;
    // 2D QList that contains Id and payment type.
    QList<QList<QString>> paymentTypesList;

    // Original promotion type data
    PromotionType oldPromotionType;
    // Updated promotion type data
    PromotionType newPromotionType;
};

#endif // PROMOTIONTYPEADDMODIFYFORM_H
