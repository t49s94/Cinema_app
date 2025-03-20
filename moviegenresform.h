/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays movie genres. Allows look up, add new, delete or update existing movie genre.

*/

#ifndef MOVIEGENRESFORM_H
#define MOVIEGENRESFORM_H

#include <QMainWindow>

#include <qmessagebox.h>

#include "login.h"
#include "moviegenresaddmodifyform.h"


namespace Ui {
class MovieGenresForm;
}

class MovieGenresForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit MovieGenresForm(QWidget *parent = nullptr);
    ~MovieGenresForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with movie genres.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtAddNew_clicked();
    void on_pbtDelete_clicked();
    void on_pbtModify_clicked();

    // Executes when push button "pbtOk" from MovieGenresAddModifyForm was clicked.
    // @param bool clicked. True if pbtOk was clicked. False if it wasn't.
    void on_is_pbtOK_clicked(bool clicked);

private:
    Ui::MovieGenresForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about movie genres.
    QSqlQueryModel *model;
};

#endif // MOVIEGENRESFORM_H
