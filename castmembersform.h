#ifndef CASTMEMBERSFORM_H
#define CASTMEMBERSFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This window displays cast members. It also allows add new, delete or modify them.

*/

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QDate>

#include "login.h"
#include "castmembersaddmodifyform.h"
#include "castmember.h"
#include "castposition.h"

namespace Ui {
class CastMembersForm;
}

class CastMembersForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    explicit CastMembersForm(QWidget *parent = nullptr);
    ~CastMembersForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with cast member information
    bool loadTable();
    // Resets all the fields in the form
    void resetFields();

private slots:

    void on_chkDOB_stateChanged(int checked);

    void on_pbtClearFields_clicked();
    void on_pbtLoadTable_clicked();
    void on_pbtAddNew_clicked();
    void on_pbtDelete_clicked();
    void on_pbtModify_clicked();

    // Executes when push button "pbtOK" from CastMembersAddModifyForm was clicked
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::CastMembersForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and functions to establish connection
    Login conn;

    // Model that contains information about cast members
    QSqlQueryModel *model;

    // Lists of positions and countries from cast members
    QStringList positionList;
    QList<int> positionIDList;
    QStringList countryList;
    QList<int> countryIDList;
};

#endif // CASTMEMBERSFORM_H
