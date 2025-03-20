/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows to add new or update existing movie genre.

*/

#ifndef MOVIEGENRESADDMODIFYFORM_H
#define MOVIEGENRESADDMODIFYFORM_H

#include <QDialog>

#include <qmessagebox.h>

#include "login.h"
#include "moviegenre.h"

namespace Ui {
class MovieGenresAddModifyForm;
}

class MovieGenresAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding a new movie genre.
    explicit MovieGenresAddModifyForm(QWidget *parent = nullptr);
    // Constructor for updating existing movie genre.
    explicit MovieGenresAddModifyForm(QWidget *parent, MovieGenre oldGenre);
    ~MovieGenresAddModifyForm();

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

private:
    Ui::MovieGenresAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // Original movie genre data
    MovieGenre oldGenre;

signals:

    // Says if push button "pbtOk" was clicked
    // @param bool clicked. True if it was clicked. False if it wasn't
    void is_pbtOK_clicked(bool clicked);
};

#endif // MOVIEGENRESADDMODIFYFORM_H
