/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class has information and functions about tickets.

*/

#include "ticket.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Ticket::Ticket()
{
    setId(0);
    this->boxOfficeTransaction = NULL;
    setBoxOfficeTransaction(new BoxOfficeTransaction);
    setAgeCategory("");
    this->seat = NULL;
    setSeat(new Seat);
    this->showtime = NULL;
    setShowtime(new Showtime);
}

Ticket::Ticket(int id, BoxOfficeTransaction *boxOfficeTransaction, QString ageCategory, Seat *seat, Showtime *showtime)
{
    setId(id);
    setBoxOfficeTransaction(boxOfficeTransaction);
    setAgeCategory(ageCategory);
    setSeat(seat);
    setShowtime(showtime);
}

Ticket::~Ticket()
{

}

//----------------------------------------------------------------------------------------------------------------------------
// Functions

// Clears pointers that class created
void Ticket::clearPointers()
{
    if(this->boxOfficeTransaction != NULL)
    {
        delete this->boxOfficeTransaction;
        this->boxOfficeTransaction = NULL;
    }

    if(this->seat != NULL)
    {
        delete this->seat;
        this->seat = NULL;
    }

    if(this->showtime != NULL)
    {
        delete this->showtime;
        this->showtime = NULL;
    }
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Ticket::getId() const
{
    return id;
}

void Ticket::setId(int value)
{
    id = value;
}

BoxOfficeTransaction *Ticket::getBoxOfficeTransaction() const
{
    return boxOfficeTransaction;
}

void Ticket::setBoxOfficeTransaction(BoxOfficeTransaction *value)
{
    if(this->boxOfficeTransaction != NULL)
    {
        delete this->boxOfficeTransaction;
    }

    boxOfficeTransaction = new BoxOfficeTransaction(*value);
}

QString Ticket::getAgeCategory() const
{
    return ageCategory;
}

void Ticket::setAgeCategory(const QString &value)
{
    ageCategory = value;
}

Seat *Ticket::getSeat() const
{
    return seat;
}

void Ticket::setSeat(Seat *value)
{
    if(this->seat != NULL)
    {
        delete this->seat;
    }

    seat = new Seat(*value);
}

Showtime *Ticket::getShowtime() const
{
    return showtime;
}

void Ticket::setShowtime(Showtime *value)
{
    if(this->showtime != NULL)
    {
        delete this->showtime;
    }

    showtime = new Showtime(*value);
}
