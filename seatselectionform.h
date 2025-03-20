/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user select seats for customers.

*/

#ifndef SEATSELECTIONFORM_H
#define SEATSELECTIONFORM_H

#include <QDialog>

#include <QPainter>
#include <QChar>
#include <qmessagebox.h>
#include <qboxlayout.h>

#include "login.h"
#include "showtime.h"
#include "theaterwidget.h"
#include "seatwidget.h"

namespace Ui {
class SeatSelectionForm;
}

class SeatSelectionForm : public QDialog
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit SeatSelectionForm(QWidget *parent, Showtime showtime, int numberPeople);
    ~SeatSelectionForm();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Draws seat in form.
    // @param QString imagePath. Background image path.
    // @param Seat *seat. Information about seat.
    void addSeat(QString imagePath, Seat *seat);

signals:
    // Says if pbtSelectSeats was clicked.
    // @param bool clicked. True if pbtSelectSeats was clicked.
    // @param QList<Seat> seatsSelected. Seats the user selected.
    void is_pbtSelectSeats_clicked(bool clicked, QList<Seat> seatsSelected);

private slots:

    void on_ptbSelectSeats_clicked();
    void on_ptbCancel_clicked();

    // Executes when a seat is clicked.
    // @param const QString buttonPressed. Button that was pressed.
    // @param const QPoint &pos. Position of click.
    // @param SeatWidget &seatWidget. Seat that was selected.
    void on_seat_clicked(const QString buttonPressed, const QPoint &pos, SeatWidget &seatWidget);

private:
    Ui::SeatSelectionForm *ui;

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Contains DB information and methods to establish connection
    Login conn;
    // Says if query type is Insert or Update
    QString queryType;

    Showtime showtime;

    // Layout for theaterWidget.
    QHBoxLayout *horizontalLayout;
    // Theater blueprints
    TheaterWidget *theaterWidget;
};

#endif // SEATSELECTIONFORM_H
