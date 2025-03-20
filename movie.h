/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about movies.

*/

#ifndef MOVIE_H
#define MOVIE_H

#include <qmainwindow.h>
#include <QDate>
#include "moviegenre.h"
#include "country.h"

class Movie
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    Movie();
    Movie(int id, QString title, int duration, QDate releaseDate, QString ageRestriction, QString synopsis, QString trailer,
          QString poster, QDate showingStartDate, QDate showingEndDate, Country country, QList<MovieGenre> genreList);

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getTitle() const;
    void setTitle(const QString &value);

    int getDuration() const;
    void setDuration(int value);

    QDate getReleaseDate() const;
    void setReleaseDate(const QDate &value);
    void setReleaseDate(const QString &value);

    QString getAgeRestriction() const;
    void setAgeRestriction(const QString &value);

    QString getSynopsis() const;
    void setSynopsis(const QString &value);

    QString getTrailer() const;
    void setTrailer(const QString &value);

    QString getPoster() const;
    void setPoster(const QString &value);

    QDate getShowingStartDate() const;
    void setShowingStartDate(const QDate &value);
    void setShowingStartDate(const QString &value);

    QDate getShowingEndDate() const;
    void setShowingEndDate(const QDate &value);
    void setShowingEndDate(const QString &value);

    QList<MovieGenre> getGenreList() const;
    void setGenreList(const QList<MovieGenre> value);

    void appendGenre(MovieGenre genre);
    MovieGenre getGenreAt(int index);

    Country getCountry() const;
    void setCountry(const Country &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString title;
    int duration;
    QDate releaseDate;
    QString ageRestriction;
    QString synopsis;
    QString trailer;
    QString poster;
    QDate showingStartDate;
    QDate showingEndDate;
    Country country;
    QList<MovieGenre> genreList;
};

#endif // MOVIE_H
