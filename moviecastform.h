/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays information about movie's cast. Allows look up, add new or delete cast members from movies.

*/

#ifndef MOVIECASTFORM_H
#define MOVIECASTFORM_H

#include <qmainwindow.h>

#include <qmessagebox.h>

#include "login.h"
#include "movie.h"
#include "moviecastaddmodifyform.h"

namespace Ui {
class MovieCastForm;
}

class MovieCastForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit MovieCastForm(QWidget *parent = nullptr);
    ~MovieCastForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Add Items to widgets that are able to contain items in form.
    // @return QString. Value "Success" if items were correctly added to widgets.
    QString addItemsToWidgets();

    // Load table with information about cast from movies.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

    // Reset fields to default value in form
    void resetFields();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    // Open MovieCastAddModifyForm to add cast members to the movie that was selected from the table.
    void on_pbtAddNew_clicked();
    // Delete cast members that were selected from the table
    void on_pbtDelete_clicked();

    void on_chkReleaseDateFrom_stateChanged(int is_checked);
    void on_chkReleaseDateTo_stateChanged(int is_checked);
    void on_chkShowingStartDate_stateChanged(int is_checked);
    void on_chkShowingEndDate_stateChanged(int is_checked);
    void on_chkDOBFrom_stateChanged(int arg1);
    void on_chkDOBTo_stateChanged(int arg1);

    // Executes when push button "pbtOK" from MovieCastAddModifyForm was clicked.
    // @param bool clicked. True if "pbtOK" was clicked. False if it wasn't
    void on_is_pbtOK_clicked(bool clicked);

    // Executes when state of an item in lvwGenres changes. Highlights checked items.
    // @param QListWidgetItem *item. Item whose state was changed.
    void highlightChecked(QListWidgetItem *item);

private:
    Ui::MovieCastForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about cast members working in movies.
    QSqlQueryModel *model;

    QStringList genresList;
    QList<int> genresIDList;

    QStringList positionList;
    QList<int> positionIDList;

    QStringList countryList;
    QList<int> countryIDList;
};

#endif // MOVIECASTFORM_H
