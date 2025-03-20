/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays movies stored in DB. Allows look up, add new, delete or update existing movie.

*/

#ifndef MOVIESFORM_H
#define MOVIESFORM_H

#include <QMainWindow>

#include <qmessagebox.h>
#include <QListWidgetItem>
#include <QList>

#include "login.h"
#include "moviesaddmodifyform.h"

namespace Ui {
class MoviesForm;
}

class MoviesForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit MoviesForm(QWidget *parent = nullptr);
    ~MoviesForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Load table with information about movies.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

    // Reset fields to default value in form.
    void resetFields();

private slots:

    void on_chkReleaseDateFrom_stateChanged(int is_checked);
    void on_chkReleaseDateTo_stateChanged(int is_checked);
    void on_chkShowingStartDate_stateChanged(int is_checked);
    void on_chkShowingEndDate_stateChanged(int arg1is_checked);

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    // Deletes selected movies in table.
    void on_pbtDelete_clicked();
    // Update selected movie from table
    void on_pbtModify_clicked();

    // Executes when is_pbtOK_clicked signal is emitted. Says if push button "pbtOk" from MoviesAddModifyForm was clicked.
    // @param bool clicked. True if pbtOk was clicked. False if it wasn't.
    void on_is_pbtOK_clicked(bool clicked);

    // Executes when item in lvwGenres is checked / unchecked. Highlights checked items.
    // @param QListWidgetItem *item. Item that was checked / unchecked.
    void highlightChecked(QListWidgetItem* item);

private:
    Ui::MoviesForm *ui;

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about movies
    QSqlQueryModel *model;

    QStringList countryList;
    // List with ids of countries
    QList<int> countryIDList;

    QStringList genresList;
    // List with ids of genres
    QList<int> genresIDList;
};

#endif // MOVIESFORM_H
