#ifndef BOXOFFICETRANSACTIONSFORM_H
#define BOXOFFICETRANSACTIONSFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window displays box office transactions that have been made.

*/

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QStandardItemModel>

#include "login.h"
#include "boxofficetransaction.h"

namespace Ui {
class BoxOfficeTransactionsForm;
}

class BoxOfficeTransactionsForm : public QMainWindow
{
    Q_OBJECT

public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    explicit BoxOfficeTransactionsForm(QWidget *parent = nullptr);
    ~BoxOfficeTransactionsForm();

    //------------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with box office transactions data
    bool loadTable();
    // Create model with box office transactions data
    void createTableModel(QSqlQuery *query);

    // Reset fields to default value
    void resetFields();

private slots:
    void on_pbtLoadTable_clicked();

    void on_pbtClearFields_clicked();

    void on_pbtDelete_clicked();

    void on_chkTransactionTimeFrom_stateChanged(int checked);

    void on_chkTransactionTimeTo_stateChanged(int checked);

    // When MyLineEdit is focussed, set mask.
    void on_ledCardNumber_focussed(bool hasFocus);

private:
    Ui::BoxOfficeTransactionsForm *ui;

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and functions to establish connection
    Login conn;

    QList<QList<QString>> paymentTypes;
    QList<QList<QString>> employees;
};

#endif // BOXOFFICETRANSACTIONSFORM_H
