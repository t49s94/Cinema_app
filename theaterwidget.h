/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class creates a widget that acts like a theater where there are seats which employee can select for people who want to buy tickets

*/

#ifndef THEATERWIDGET_H
#define THEATERWIDGET_H

#include <QObject>
#include <QFrame>
#include <QLabel>
#include <QtWidgets>

#include "seatwidget.h"

class SeatWidget;
class TheaterWidget : public QFrame
{
    Q_OBJECT
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    TheaterWidget(QWidget *parent, int numberPeople);

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Add a SeatWidget to the class
    // @param SeatWidget *label. Widget that's gonna be added
    // @param QString path. Path of the image that SeatWidget will have.
    // @param QPoint pos. Position where the SeatWidget will be.
    // @param QString text. Row and Column of seat.
    void addImage(SeatWidget * label, QString path, QPoint pos, QString text);
    // Draw a text over SeatWidget
    // @param SeatWidget *label. Widget that will have the text drawn over.
    // @param QString path. Path of the image that SeatWidget will have.
    // @param QString text. Text that will be drawn over the SeatWidget.
    void addTextToImage(SeatWidget *image, QString path, QString text);

    // Find a SeatWidget from QList.
    // @param QList<SeatWidget*> labels.
    // @param int idNumbers. ID from SeatWidget that wants to be found
    // @return int. Index of SeatWidget found in QList.
    int getSeatIndex(QList<SeatWidget*> labels, int idNumber);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getNumberPeople() const;
    void setNumberPeople(int value);

    QList<int> *getSelectedSeatIdNumbers() const;
    void setSelectedSeatIdNumbers(QList<int> *value);

    QList<Seat> *getSelectedSeats() const;
    void setSelectedSeats(QList<Seat> *value);

protected:

    void mousePressEvent(QMouseEvent *event) override;

signals:

    // Send information about seat that was selected.
    // @param const QString buttonPressed. Button that was pressed.
    // @param const QPoint &pos. Mouse position.
    // @param SeatWidget &seatWidget. Seat that was clicked.
    void is_pressed(const QString buttonPressed, const QPoint &pos, SeatWidget &seatWidget);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    // Id number of SeatWidget that was currently selected .
    int currentIdNumber;
    // Increases by one each time a SeatWidget is created. Ensures unique Id number.
    int lastIdNumber;

    QList<int> *selectedSeatIdNumbers;
    QList<Seat> *selectedSeats;
    // Number of people who wants to buy tickets
    int numberPeople;
};

#endif // THEATERWIDGET_H
