#ifndef CUSTOMERSADDMODIFYFORM_H
#define CUSTOMERSADDMODIFYFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window add or update existing customer.

*/

#include <QDialog>

#include "login.h"
#include "customer.h"
#include <qmessagebox.h>
#include <QListWidgetItem>

namespace Ui {
class CustomersAddModifyForm;
}

class CustomersAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    // Constructor for adding new customer
    explicit CustomersAddModifyForm(QWidget *parent, QList<int> customerTypeIDList, QStringList customerTypeList);
    // Constructor for updating existing customer
    explicit CustomersAddModifyForm(QWidget *parent, QList<int> customerTypeIDList, QStringList customerTypeList, Customer oldCustomer);
    ~CustomersAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initialize some features of this form.
    // @param QList<int> customerTypeIDList. List with IDs of customer types.
    // @param QStringList customerTypeList. List with names of customer types.
    void initializeForm(QList<int> customerTypeIDList, QStringList customerTypeList);

    // Validates the fields in form.
    // @return bool. True if fields have correct data
    bool validateFields();
    // Checks if required fields are not blank
    // @return bool. True if all required fields were filled out.
    bool areFieldsNotBlank();

    // Create a new member card for customer.
    // @return bool. True if new card was succesfully created.
    bool createNewMemberCard();

    // Sets the text of update query.
    // @param QString* queryText. Holds the update query text.
    // @return bool. False if there were no changes made in any field.
    bool setUpdateQueryText(QString* queryText);

private:
    Ui::CustomersAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    QList<int> customerTypeIDList;
    QStringList customerTypeList;

    // Original data of customer
    Customer oldCustomer;
    // New data of customer
    Customer newCustomer;

signals:
    void is_pbtOK_clicked(bool clicked);

private slots:
    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

    // Executes when ledPhone is foccussed in or out.
    void on_ledPhone_focussed(bool hasFocus);
};

#endif // CUSTOMERSADDMODIFYFORM_H
