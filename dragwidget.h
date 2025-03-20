#ifndef DRAGWIDGET_H
#define DRAGWIDGET_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class creates a widget were items (seats) can be dragged and dropped around.

*/

#include <QFrame>
#include <QLabel>
#include <QObject>

#include "seatwidget.h"

QT_BEGIN_NAMESPACE
// We define the next 3 classes because the compiler doesn't know them at this point so It will throw and error if we didn't
class QDragEnterEvent;
class QDropEvent;
QT_END_NAMESPACE

//! [0]
class SeatWidget;
class DragWidget : public QFrame
{
    Q_OBJECT
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    explicit DragWidget(QWidget *parent = nullptr);

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Add a SeatWidget to the class
    // @param SeatWidget *label. Widget that's gonna be added
    // @param QString path. Path of the image that SeatWidget will have.
    // @param QPoint pos. Position where the SeatWidget is gonna be.
    void addImage(SeatWidget * label, QString path, QPoint pos);
    // Add a SeatWidget to the class which will have a text on it
    // @param SeatWidget *label. Widget that's gonna be added
    // @param QString path. Path of the image that SeatWidget will have.
    // @param QPoint pos. Position where the SeatWidget is gonna be.
    // @param QString text. Text that will be drawn over the SeatWidget.
    void addImage(SeatWidget * label, QString path, QPoint pos, QString text);

    // Draw a text over SeatWidget
    // @param SeatWidget *label. Widget that will have the text drawn over.
    // @param QString path. Path of the image that SeatWidget will have.
    // @param QString text. Text that will be drawn over the SeatWidget.
    void addTextToImage(SeatWidget *image, QString path, QString text);

    // Delete SeatWidget from draggable container
    // @return Seat. Seat that was deleted.
    Seat removeSeat();

    // Find a SeatWidget from QList.
    // @param QList<SeatWidget*> labels.
    // @param int idNumbers. ID from SeatWidget that wants to be found
    // @return int. Index of SeatWidget found in QList.
    int getSeatIndex(QList<SeatWidget*> labels, int idNumber);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getPreviousIdNumber() const;
    void setPreviousIdNumber(int value);

protected:

    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

signals:

    // Signal emitted when SeatWidget was pressed.
    // @param const QString buttonPressed.
    // @param const QPoint &pos. Position of mouse click.
    // @param SeatWidget &seatWidget. SeatWidget that was pressed.
    // @return int. Index of SeatWidget found in QList.
    void is_pressed(const QString buttonPressed, const QPoint &pos, SeatWidget &seatWidget);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    QList<SeatWidget*> newSeatsList;
    // Id number of SeatWidget that was selected currently
    int currentIdNumber;
    // Id number of SeatWidget that was selected before the current one
    int previousIdNumber;

    // Increases by one each time a SeatWidget is created. Ensures unique Id number.
    int lastIdNumber;

    Seat *currentSeat;
    QString currentSeatName;

    //QPoint highlightPoint;
};
//! [0]

#endif // DRAGWIDGET_H
