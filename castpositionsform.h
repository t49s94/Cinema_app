#ifndef CASTPOSITIONSFORM_H
#define CASTPOSITIONSFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window displays cast positions stored in DB.

*/

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <qstandarditemmodel.h>

#include "login.h"
#include "castpositionsaddmodifyform.h"
#include "castposition.h"

namespace Ui {
class CastPositionsForm;
}

class CastPositionsForm : public QMainWindow
{
    Q_OBJECT

public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    explicit CastPositionsForm(QWidget *parent = nullptr);
    ~CastPositionsForm();

    //------------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with information about cast positions.
    // @return bool. True if table was loaded succesfully.
    bool loadTable();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtDelete_clicked();
    void on_pbtAddNew_clicked();
    void on_pbtModify_clicked();

    // Executes when push button "pbtOK" from CastPositionsAddModifyForm is clicked
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::CastPositionsForm *ui;

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and functions to establish connection
    Login conn;
    // Model that contains information about cast positions
    QSqlQueryModel *model;

signals:

};

#endif // CASTPOSITIONSFORM_H
