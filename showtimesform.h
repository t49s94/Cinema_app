/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays information about showtimes stored in DB. Allows look up, add new, delete or update existing showtime.

*/

#ifndef SHOWTIMESFORM_H
#define SHOWTIMESFORM_H

#include <QMainWindow>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QStandardItemModel>

#include "login.h"
#include "showtimeaddmodifyform.h"

namespace Ui {
class ShowtimesForm;
}

class ShowtimesForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit ShowtimesForm(QWidget *parent = nullptr);
    ~ShowtimesForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Loads table with information about showtimes.
    // @return bool. True if table was successfully loaded.
    bool loadTable();
    // Reset fields to their default state.
    void resetFields();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    // Deletes selected showtimes from table
    void on_pbtDelete_clicked();
    // Updates selected showtime
    void on_pbtModify_clicked();

    // Executes when is_pbtSave_clicked signal is emitted.
    // @param bool clicked. True if push button pbtSave was clicked.
    void on_is_pbtSave_clicked(bool clicked);

    void on_chkStartAt_stateChanged(int checked);
    void on_chkEndAt_stateChanged(int checked);
    void on_chkShowDateFrom_stateChanged(int checked);
    void on_chkShowDateTo_stateChanged(int checked);

private:
    Ui::ShowtimesForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about showtimes.
    QStandardItemModel* model;

    // 2D list with "Id" and "Theater name"
    QList<QList<QString>> theatersList;
};

#endif // SHOWTIMESFORM_H



