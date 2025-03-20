/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class has information and functions about tickets.

*/

#ifndef TICKET_H
#define TICKET_H

#include "boxofficetransaction.h"
#include "seat.h"
#include "showtime.h"

class Ticket
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Ticket();
    Ticket(int id, BoxOfficeTransaction* boxOfficeTransaction, QString ageCategory, Seat* seat, Showtime* showtime);

    ~Ticket();

    //----------------------------------------------------------------------------------------------------------------------------
    // Functions

    // Clears pointers that class created
    void clearPointers();

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    BoxOfficeTransaction *getBoxOfficeTransaction() const;
    void setBoxOfficeTransaction(BoxOfficeTransaction *value);

    QString getAgeCategory() const;
    void setAgeCategory(const QString &value);

    Seat *getSeat() const;
    void setSeat(Seat *value);

    Showtime *getShowtime() const;
    void setShowtime(Showtime *value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    BoxOfficeTransaction* boxOfficeTransaction;
    QString ageCategory;
    Seat* seat;
    Showtime* showtime;
};

#endif // TICKET_H
