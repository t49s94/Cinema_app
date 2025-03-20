/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class inherits from QLabel and acts like a seat in a theater.

*/

#ifndef SEATWIDGET_H
#define SEATWIDGET_H

#include <QLabel>

#include "seat.h"
#include "dragwidget.h"

class SeatWidget : public QLabel
{
    Q_OBJECT

public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    SeatWidget();
    SeatWidget(QWidget *parent);
    SeatWidget(QWidget *parent, int id, Seat *seat, QString imagePath);

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Initializes SeatWidget.
    // @param QWidget *parent.
    // @param int idNumber. Unique Id for Seat.
    // @param Seat *seat. Information about seat.
    // @param QString imagePath. Background image for Seat.
    void initialize(QWidget *parent, int idNumber, Seat *seat, QString imagePath);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getIdNumber() const;
    void setIdNumber(int value);

    Seat *getSeat() const;
    void setSeat(Seat *value);

    QString getImagePath() const;
    void setImagePath(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int idNumber;
    QString imagePath;
    Seat* seat;

};

#endif // SEATWIDGET_H
