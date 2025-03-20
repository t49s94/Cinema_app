/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user to add or update existing theater type.

*/

#ifndef THEATERTYPEADDMODIFYFORM_H
#define THEATERTYPEADDMODIFYFORM_H

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "theatertype.h"

namespace Ui {
class TheaterTypeAddModifyForm;
}

class TheaterTypeAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding a new theater type.
    explicit TheaterTypeAddModifyForm(QWidget *parent = nullptr);
    // Constructor for updating existing theater type.
    explicit TheaterTypeAddModifyForm(QWidget *parent, TheaterType oldTheaterType);
    ~TheaterTypeAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Checks if required fields were filled out.
    // @return bool. True if all required fields were filled out
    bool areFieldsNotBlank();

    // Creates query text for update.
    // @param QString* queryText. QString that contains update query text
    // @return bool. False if there were no changes made.
    bool setUpdateQueryText(QString* queryText);

signals:

    // Says if pbtOk was clicked.
    // @param bool clicked. True if pbtOk was clicked.
    void is_pbtOK_clicked(bool clicked);

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::TheaterTypeAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // Origianl theater type data
    TheaterType oldTheaterType;
    // Updated theater type data
    TheaterType newTheaterType;
};

#endif // THEATERTYPEADDMODIFYFORM_H
