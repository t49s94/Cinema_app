/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This is the main window for the general manager and manager. Allows access to all tables in DB for looking up, add, delete and update records.

*/

#ifndef GENERALMANAGERFORM_H
#define GENERALMANAGERFORM_H

#include <QMainWindow>

#include <qmdisubwindow.h>

#include "castpositionsform.h"
#include "castmembersform.h"
#include "employee.h"
#include "moviegenresform.h"
#include "moviesform.h"
#include "moviecastform.h"
#include "customertypeform.h"
#include "customersform.h"
#include "employeetypesform.h"
#include "employeesform.h"
#include "theatertypesform.h"
#include "paymenttypesform.h"
#include "promotiontypeform.h"
#include "couponsform.h"
#include "promotionsform.h"
#include "theatersform.h"
#include "showtimesform.h"
#include "boxofficetransactionsform.h"
#include "boxofficetransactionaddform.h"

namespace Ui {
class GeneralManagerForm;
}

class GeneralManagerForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit GeneralManagerForm(QWidget *parent, Employee employee);
    ~GeneralManagerForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load a subwindow for this window.
    // @param QWidget *widget. Window that will be shown.
    void loadSubWindow(QWidget *widget);

private slots:

    void on_actionCast_members_triggered();
    void on_actionCast_positions_triggered();
    void on_actionMovie_genres_triggered();
    void on_actionMovies_triggered();
    void on_actionCast_triggered();
    void on_actionCustomer_types_triggered();
    void on_actionCustomers_triggered();
    void on_actionEmployee_positions_triggered();
    void on_actionEmployees_triggered();
    void on_actionTheater_types_triggered();
    void on_actionPayment_types_triggered();
    void on_actionPromotion_types_triggered();
    void on_actionCoupons_triggered();
    void on_actionPromotions_triggered();
    void on_actionTheaters_triggered();
    void on_actionShowtimes_triggered();
    void on_actionAll_transactions_triggered();
    void on_actionNew_transaction_triggered();
    void on_actionLog_out_triggered();

private:
    Ui::GeneralManagerForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Employee that is using the app.
    Employee employee;

signals:
    void is_menuLog_out_triggered(bool clicked);
};

#endif // GENERALMANAGERFORM_H
