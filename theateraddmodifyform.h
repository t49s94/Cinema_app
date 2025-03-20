/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user to add new or update existing theate.

*/

#ifndef THEATERADDMODIFYFORM_H
#define THEATERADDMODIFYFORM_H

#include <QDialog>

#include <qsqlquerymodel.h>
#include <qmessagebox.h>
#include <QPainter>
#include <QChar>
#include <qboxlayout.h>

#include "login.h"
#include "theater.h"
#include "dragwidget.h"
#include "seatwidget.h"

namespace Ui {
class TheaterAddModifyForm;
}

class TheaterAddModifyForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    // Constructor for adding new theater
    explicit TheaterAddModifyForm(QWidget *parent, QList<QList<QString>> theaterTypesList);
    // Constructor for updating existing theater
    explicit TheaterAddModifyForm(QWidget *parent, QList<QList<QString>> theaterTypesList, Theater oldTheater);
    ~TheaterAddModifyForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initializes some features of form.
    // @param QList<QList<QString>> *theaterTypesList. 2D List with "Id" and "Theater type name".
    void initialize(QList<QList<QString> > *theaterTypesList);

    // Checks if required fields were filled out.
    // @return bool. True if all required fields were filled out.
    bool areFieldsNotBlank();

    // Draws a new seat in theater.
    // @param QString imagePath. Background image of seat.
    // @param QPoint pos. Position where seat is located.
    void addSeat(QString imagePath, QPoint pos);
    // Draws an existing seat in theater
    // @param QString imagePath. Background image of seat.
    // @param QPoint pos. Position where seat is located.
    void addSeat(QString imagePath, QPoint pos, QString text);
    // Saves seats in theater.
    // @param QSqlQuery *query.
    bool saveSeats(QSqlQuery* query);

    // Creates query text for update.
    // @param QString* queryText. QString that contains update query text
    // @return bool. False if there were no changes made.
    bool setUpdateQueryText(QString* queryText, QStringList *setsList);

signals:

    // Says if pbtSave was clicked.
    // @param bool clicked. True if pbtSave was clicked.
    void is_pbtSave_clicked(bool clicked);

private slots:

    void on_pbtSave_clicked();
    void on_pbtCancel_clicked();

    void on_pbtAddSeat_clicked();
    void on_pbtModifyRowColumn_clicked();
    void on_pbtAddHandicapSeat_clicked();
    void on_pbtRemoveSeat_clicked();

    void on_ledSeatRow_editingFinished();

    // Executed when user clicks on seat.
    // @param QString buttonPressed. Button that was pressed.
    // @param const QPoint &pos. Position where mouse was.
    // @param SeatWidget &seatWidget. Seat that was clicked
    void on_seat_clicked(const QString buttonPressed, const QPoint &pos, SeatWidget &seatWidget);

    // Executes when ledSeatRow is clicked. Sets cursor position to 0 so user won't have problems typing. Since ledSeatRow has mask "a",
    // typing will be difficult because user has to make sure he's in position 0 to input row. Otherwise mask won't allow him to type anything.
    void on_ledSeatRow_pressed();

private:
    Ui::TheaterAddModifyForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    // 2D List with "Id" and "Theater type name"
    QList<QList<QString>> theaterTypesList;

    // Original theater data
    Theater oldTheater;
    // Update theater data
    Theater newTheater;

    // Layout for dragWidget
    QHBoxLayout *horizontalLayout;
    // Canvas where seats are gonna be drawn and drag and drop is allowed
    DragWidget *dragWidget;

    // List of current seats in theater
    QList<Seat> oldSeatList;
     // List of Ids of current seats in theater
    QList<int> oldSeatIdNumberList;

    // Lists that wants to be deleted from DB
    QList<Seat> removedSeatsList;

    // Id number of seat that is currently selected
    int currentIdNumber;
    // Seat that is currently selected
    Seat currentSeat;

};

#endif // THEATERADDMODIFYFORM_H
