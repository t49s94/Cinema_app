/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about movie genres.

*/

#include "moviegenre.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MovieGenre::MovieGenre()
{
    id = 0;
    genre = "";
}

MovieGenre::MovieGenre(int id, QString genre)
{
    this->id = id;
    this->genre = genre;
}

MovieGenre::~MovieGenre()
{

}

//----------------------------------------------------------------------------------------------------------------------------
// Getters / Setters

int MovieGenre::getId() const
{
    return id;
}

void MovieGenre::setId(int value)
{
    id = value;
}

QString MovieGenre::getGenre() const
{
    return genre;
}

void MovieGenre::setGenre(const QString &value)
{
    genre = value;
}
