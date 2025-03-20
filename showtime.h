/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class has information and functions about Showtimes.

*/

#ifndef SHOWTIME_H
#define SHOWTIME_H

#include <QTime>
#include <QDate>

#include "movie.h"
#include "theater.h"

class Showtime
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Showtime();
    Showtime(int id, Movie movie, QTime startAt, QTime endAt, QDate showDate, Theater theater);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    Movie getMovie() const;
    void setMovie(const Movie &value);

    QTime getStartAt() const;
    void setStartAt(const QTime &value);
    void setStartAt(const QString &value);

    QTime getEndAt() const;
    void setEndAt(const QTime &value);
    void setEndAt(const QString &value);

    QDate getShowDate() const;
    void setShowDate(const QDate &value);
    void setShowDate(const QString &value);

    Theater getTheater() const;
    void setTheater(const Theater &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    Movie movie;
    QTime startAt;
    QTime endAt;
    QDate showDate;
    Theater theater;

};

#endif // SHOWTIME_H
