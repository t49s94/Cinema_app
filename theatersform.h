#ifndef THEATERSFORM_H
#define THEATERSFORM_H
/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window displays the theaters stored in DB. Allows look up, add new, delete or updating existing theater.

*/

#include <QMainWindow>

#include <qboxlayout.h>
#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QStandardItemModel>

#include "login.h"
#include "theater.h"
#include "theateraddmodifyform.h"

namespace Ui {
class TheatersForm;
}

class TheatersForm : public QMainWindow
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit TheatersForm(QWidget *parent = nullptr);
    ~TheatersForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Loads table with information about theaters.
    // @return bool. True if table was successfully loaded.
    bool loadTable();

    // Reset form's fields to default state.
    void resetFields();

private slots:

    void on_pbtLoadTable_clicked();
    void on_pbtClearFields_clicked();
    void on_pbtAddNew_clicked();
    // Delete selected theaters from table
    void on_pbtDelete_clicked();
    // Update selected theater from table
    void on_pbtModify_clicked();

    // Executed when is_pbtSave_clicked is emitted.
    // @param bool clicked. True if push button "pbtSave" was clicked
    void on_is_pbtSave_clicked(bool clicked);

private:
    Ui::TheatersForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Model that contains information about theaters
    QStandardItemModel* model;

    // 2D List with "Ids" and "Theater type name"
    QList<QList<QString>> theaterTypesList;
};

#endif // THEATERSFORM_H

