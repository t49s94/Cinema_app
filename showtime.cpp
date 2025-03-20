/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class has information and functions about Showtimes.

*/

#include "showtime.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Showtime::Showtime()
{
    setId(0);
    setMovie(Movie());
    setStartAt(QTime::currentTime());
    setEndAt(QTime::currentTime());
    setShowDate(QDate::currentDate());
    setTheater(Theater());
}

Showtime::Showtime(int id, Movie movie, QTime startAt, QTime endAt, QDate showDate, Theater theater)
{
    setId(id);
    setMovie(movie);
    setStartAt(startAt);
    setEndAt(endAt);
    setShowDate(showDate);
    setTheater(theater);
}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Showtime::getId() const
{
    return id;
}

void Showtime::setId(int value)
{
    id = value;
}

Movie Showtime::getMovie() const
{
    return movie;
}

void Showtime::setMovie(const Movie &value)
{
    movie = value;
}

QTime Showtime::getStartAt() const
{
    return startAt;
}

void Showtime::setStartAt(const QTime &value)
{
    startAt = value;
}

void Showtime::setStartAt(const QString &value)
{
    startAt = QTime::fromString(value, "hh:mm:ss");
}

QTime Showtime::getEndAt() const
{
    return endAt;
}

void Showtime::setEndAt(const QTime &value)
{
    endAt = value;
}

void Showtime::setEndAt(const QString &value)
{
    endAt = QTime::fromString(value, "hh:mm:ss");
}

QDate Showtime::getShowDate() const
{
    return showDate;
}

void Showtime::setShowDate(const QDate &value)
{
    showDate = value;
}

void Showtime::setShowDate(const QString &value)
{
    showDate = QDate::fromString(value, "yyyy-MM-dd");;
}

Theater Showtime::getTheater() const
{
    return theater;
}

void Showtime::setTheater(const Theater &value)
{
    theater = value;
}
