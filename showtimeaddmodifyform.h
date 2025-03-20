/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user to add new or update existing showtime.

*/

#ifndef SHOWTIMEADDMODIFYFORM_H
#define SHOWTIMEADDMODIFYFORM_H

#include <QDialog>

#include <qstandarditemmodel.h>
#include <qmessagebox.h>
#include <QListWidgetItem>

#include "login.h"
#include "showtime.h"


namespace Ui {
class ShowtimeAddModifyForm;
}

class ShowtimeAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new showtime
    explicit ShowtimeAddModifyForm(QWidget *parent, QList<QList<QString>> theatersList);
    // Constructor for updating existing showtime
    explicit ShowtimeAddModifyForm(QWidget *parent, QList<QList<QString>> theatersList, Showtime oldShowtime);
    ~ShowtimeAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initialize a few features of form.
    // @param QList<QList<QString>>* theatersList. 2D List with "ID" and "Theater name"
    void initialize(QList<QList<QString>>* theatersList);

    // Checks if required fields were filled out.
    // @return bool. True if required fields were filled out.
    bool areFieldsNotBlank();
    // Checks if fields have correct data
    // @return bool. True if fields have correct data.
    bool validateFields();

    // Creates query text for update.
    // @param QString* queryText. QString that contains update query text
    // @return bool. False if there were no changes made.
    bool setUpdateQueryText(QString* queryText);

private:
    Ui::ShowtimeAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // 2D List with "Id" and "Theater name"
    QList<QList<QString>> theatersList;

    // Model that contains information about movies
    QStandardItemModel* model;

    // Original showtime data
    Showtime oldShowtime;
    // Updated showtime data
    Showtime newShowtime;

signals:

    // Says if pbtSave was clicked.
    // @param bool clicked. True if pbtSave was clicked.
    void is_pbtSave_clicked(bool clicked);

private slots:

    void on_pbtOk_clicked();
    void on_pbtCancel_clicked();

    // Sets the movie of the item that was double clicked.
    // @param const QModelIndex &index. Double clicked item.
    void on_tvwTable_doubleClicked(const QModelIndex &index);
};

#endif // SHOWTIMEADDMODIFYFORM_H
