/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows add new or update list of cast that works in a movie.

*/

#ifndef MOVIECASTADDMODIFYFORM_H
#define MOVIECASTADDMODIFYFORM_H

#include <QDialog>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QDate>

#include "login.h"
#include "movie.h"
#include "castmembersaddmodifyform.h"
#include "castmember.h"
#include "castposition.h"

namespace Ui {
class MovieCastAddModifyForm;
}

class MovieCastAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit MovieCastAddModifyForm(QWidget *parent, QStringList countryList, QStringList positionList, Movie movie);
    ~MovieCastAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with cast members so the user can select the ones who will work in the movie.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

    // Reset fields in form
    void resetFields();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();

    void on_chkDOB_stateChanged(int checked);

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

signals:

    // Emitted when push button pbtOK is clicked.
    // @param bool clicked. True if it was clicked. False if it wasn't.
    void is_pbtOK_clicked(bool clicked);

private:
    Ui::MovieCastAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about cast members
    QSqlQueryModel *model;

    QStringList positionList;
    QStringList countryList;

    // Movie which cast members are gonna be added to
    Movie movie;
};

#endif // MOVIECASTADDMODIFYFORM_H
