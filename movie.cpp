#include "movie.h"

/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about movies.

*/

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

Movie::Movie()
{
    this->id = 0;
    this->title = "";
    this->duration = 0;
    this->releaseDate = QDate::currentDate();
    this->ageRestriction = "";
    this->synopsis = "";
    this->trailer = "";
    this->poster = "";
    this->showingStartDate = QDate::currentDate();
    this->showingEndDate = QDate::currentDate();
}

Movie::Movie(int id, QString title, int duration, QDate releaseDate, QString ageRestriction, QString synopsis, QString trailer,
             QString poster, QDate showingStartDate, QDate showingEndDate, Country country, QList<MovieGenre> genreList)
{
    this->id = id;
    this->title = title;
    this->duration = duration;
    this->releaseDate = releaseDate;
    this->ageRestriction = ageRestriction;
    this->synopsis = synopsis;
    this->trailer = trailer;
    this->poster = poster;
    this->showingStartDate = showingStartDate;
    this->showingEndDate = showingEndDate;
    this->country = country;
    this->genreList = genreList;
    this->country = country;

}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int Movie::getId() const
{
    return id;
}

void Movie::setId(int value)
{
    id = value;
}

QString Movie::getTitle() const
{
    return title;
}

void Movie::setTitle(const QString &value)
{
    title = value;
}

int Movie::getDuration() const
{
    return duration;
}

void Movie::setDuration(int value)
{
    duration = value;
}

QDate Movie::getReleaseDate() const
{
    return releaseDate;
}

void Movie::setReleaseDate(const QDate &value)
{
    releaseDate = value;
}

void Movie::setReleaseDate(const QString &value)
{
    releaseDate = QDate::fromString(value, "yyyy-MM-dd");
}

QString Movie::getAgeRestriction() const
{
    return ageRestriction;
}

void Movie::setAgeRestriction(const QString &value)
{
    ageRestriction = value;
}

QString Movie::getSynopsis() const
{
    return synopsis;
}

void Movie::setSynopsis(const QString &value)
{
    synopsis = value;
}

QString Movie::getTrailer() const
{
    return trailer;
}

void Movie::setTrailer(const QString &value)
{
    trailer = value;
}

QString Movie::getPoster() const
{
    return poster;
}

void Movie::setPoster(const QString &value)
{
    poster = value;
}

QDate Movie::getShowingStartDate() const
{
    return showingStartDate;
}

void Movie::setShowingStartDate(const QDate &value)
{
    showingStartDate = value;
}

void Movie::setShowingStartDate(const QString &value)
{
    showingStartDate = QDate::fromString(value, "yyyy-MM-dd");
}

QDate Movie::getShowingEndDate() const
{
    return showingEndDate;
}

void Movie::setShowingEndDate(const QDate &value)
{
    showingEndDate = value;
}

void Movie::setShowingEndDate(const QString &value)
{
    showingEndDate = QDate::fromString(value, "yyyy-MM-dd");
}

QList<MovieGenre> Movie::getGenreList() const
{
    return genreList;
}

void Movie::setGenreList(const QList<MovieGenre> value)
{
    genreList = value;
}

void Movie::appendGenre(MovieGenre genre)
{
    genreList.append(genre);
}

MovieGenre Movie::getGenreAt(int index)
{
    return genreList.at(index);
}

Country Movie::getCountry() const
{
    return country;
}

void Movie::setCountry(const Country &value)
{
    country = value;
}






