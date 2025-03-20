/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This window allows user select seats for customers.

*/

#include "seatselectionform.h"
#include "ui_seatselectionform.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

SeatSelectionForm::SeatSelectionForm(QWidget *parent, Showtime showtime, int numberPeople) :
    QDialog(parent),
    ui(new Ui::SeatSelectionForm)
{
    ui->setupUi(this);
    // Prevents user from resizing Dialog
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->showtime = showtime;

    horizontalLayout = new QHBoxLayout(ui->widTheater);
    theaterWidget = new TheaterWidget(ui->widTheater, numberPeople);
    // Signal is_pressed is emitted when user clicks on theaterWidget. Executes Slot on_seat_clicked.
    connect (theaterWidget, SIGNAL (is_pressed(const QString, const QPoint&, SeatWidget&)), this, SLOT(on_seat_clicked(const QString, const QPoint&, SeatWidget&)));

    horizontalLayout->addWidget(theaterWidget);

    QPixmap pixmap(":/Images/movie theater.png");
    ui->lblTheater->setPixmap(pixmap);

    if(!conn.connOpen())
    {
        QMessageBox::warning(this, "Error", "Couldn't load seats!\nError: " + conn.nativeErrorCode());
        return;
    }

    QSqlQuery query;

    // Gets seats from showtime. "WHERE s.theater_id = :theater_id" makes sure the seats belong to the theater where the showtime is.
    query.prepare("SELECT s.seat_id, s.theater_id, s.seat_row, s.seat_column, s.handicap, s.position_x, s.position_y, t.showtime_id "
                  "FROM seat s "
                  "LEFT JOIN ticket t "
                  "ON s.seat_id = t.seat_id "
                  "WHERE s.theater_id = :theater_id");

    query.bindValue(":theater_id", showtime.getTheater().getId());

    if(!query.exec())
    {
        conn.connClose();
        QMessageBox::warning(this, "Error", "Couldn't load seats!\nError: " + query.lastError().nativeErrorCode());
        close();
    }

    // Draws all seats of showtime
    while(query.next())
    {
        QString row = query.value(2).toString();

        Seat *seat = new Seat(query.value(0).toInt(),
                              Theater(showtime.getTheater().getId(), "", TheaterType()),
                              row.toStdString().c_str()[0], query.value(3).toString(),
                              query.value(4).toBool(), query.value(5).toInt(),
                              query.value(6).toInt());

        QString imagePath = "";

        // If t.showtime_id is null means no person has selected that seat for that showtime. So seat is available
        if(query.value(7).toInt() != showtime.getId())
            if(query.value(4).toBool())
                imagePath = ":/Images/available handicap seat.png";
        else
                imagePath = ":/Images/available seat.png";
        else
            if(query.value(4).toBool())
                imagePath = ":/Images/occupied handicap seat.png";
        else
                imagePath = ":/Images/occupied seat.png";

        addSeat(imagePath, seat);

    }

    conn.connClose();

    ui->lblTitle->setText("MOVIE: " + showtime.getMovie().getTitle().left(40) + "   AT: " + showtime.getStartAt().toString("hh:mm:ss") +
                          " " + showtime.getShowDate().toString("MM-dd-yyyy"));
}

SeatSelectionForm::~SeatSelectionForm()
{
    delete theaterWidget;
    delete horizontalLayout;

    delete ui;
}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Draws seat in form.
// @param QString imagePath. Background image path.
// @param Seat *seat. Information about seat.
void SeatSelectionForm::addSeat(QString imagePath, Seat *seat)
{
    SeatWidget* seatWidget = new SeatWidget(theaterWidget, 0, seat, imagePath);
    //seatWidget->setObjectName("oldSeat");

    if(imagePath != ":/Images/available seat.png")
        seatWidget->getSeat()->setHandicap(true);

    theaterWidget->addImage(seatWidget, imagePath, QPoint(seat->getPositionX(), seat->getPositionY()),
                            QChar(seat->getSeatRow()) + seat->getSeatColumn());
}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

void SeatSelectionForm::on_ptbSelectSeats_clicked()
{
    // Means ptbSelectSeats was clicked
    emit is_pbtSelectSeats_clicked(true, *theaterWidget->getSelectedSeats());
    close();
}

void SeatSelectionForm::on_ptbCancel_clicked()
{
    // Means ptbSelectSeats wasn't clicked
    emit is_pbtSelectSeats_clicked(false, QList<Seat>());
    close();
}

// Executes when a seat is clicked.
// @param const QString buttonPressed. Button that was pressed.
// @param const QPoint &pos. Position of click.
// @param SeatWidget &seatWidget. Seat that was selected.
void SeatSelectionForm::on_seat_clicked(const QString buttonPressed, const QPoint &pos, SeatWidget &seatWidget)
{
    // Enable ptbSelectSeats when a seat was given to all customers who wants to watch the movie
    if(theaterWidget->getSelectedSeatIdNumbers()->length() == theaterWidget->getNumberPeople())
        ui->ptbSelectSeats->setEnabled(true);
    else
        ui->ptbSelectSeats->setEnabled(false);
}
