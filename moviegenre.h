/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class contains information and functions about movie genres.

*/

#ifndef MOVIEGENRE_H
#define MOVIEGENRE_H

#include <qmainwindow.h>

class MovieGenre
{
public:

    //----------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructor

    MovieGenre();
    MovieGenre(int id, QString genre);
    ~MovieGenre();

    //----------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getGenre() const;
    void setGenre(const QString &value);

private:

    //----------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString genre;
};

#endif // MOVIEGENRE_H
