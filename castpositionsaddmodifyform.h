#ifndef CASTPOSITIONSADDMODIFYFORM_H
#define CASTPOSITIONSADDMODIFYFORM_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This windows allows add or update an exisiting cast position.

*/

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "castposition.h"

namespace Ui {
class CastPositionsAddModifyForm;
}

class CastPositionsAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    // Constructor for adding new cast position
    explicit CastPositionsAddModifyForm(QWidget *parent = nullptr);
    // Constructor for updating existing position
    explicit CastPositionsAddModifyForm(QWidget *parent, CastPosition oldCastPosition);
    ~CastPositionsAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Returns true if fields are not blank
    bool areFieldsNotBlank();

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::CastPositionsAddModifyForm *ui;

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and functions to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    CastPosition oldCastPosition;

signals:
    // Emits when push button "pbtOK" is clicked
    // @param bool clicked. True if "pbtOK" was clicked
    void is_pbtOK_clicked(bool clicked);

};

#endif // CASTPOSITIONSADDMODIFYFORM_H
