#ifndef CUSTOMERTYPEFORM_H
#define CUSTOMERTYPEFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays the customer types stored in DB. Allows to look up, add new, delete or update.

*/

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

#include "login.h"
#include "customertypesaddmodifyform.h"

namespace Ui {
class CustomerTypeForm;
}

class CustomerTypeForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit CustomerTypeForm(QWidget *parent = nullptr);
    ~CustomerTypeForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Loads table with information about customer types.
    // @return bool. True if table was sucessfully loaded.
    bool loadTable();

private slots:

    void on_pbtLoadTable_clicked();
    // Delete selected rows from table
    void on_pbtDelete_clicked();
    void on_pbtAddNew_clicked();
    // Update selected row.
    void on_pbtModify_clicked();

    // Executes when is_pbtOK_clicked signal is emitted from CustomerTypesAddModifyForm.
    // @param bool clicked. True if push button "pbtOK" was clicked.
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::CustomerTypeForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about customer types
    QSqlQueryModel *model;

};

#endif // CUSTOMERTYPEFORM_H
