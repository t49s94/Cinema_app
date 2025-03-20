#ifndef CASTPOSITION_H
#define CASTPOSITION_H

/*

Developer: Sergio Eduardo Santillana Lopez.
Last update: 12/16/2020.

This class contains information about cast positions.

*/

#include <qmainwindow.h>

class CastPosition
{

public:

    //------------------------------------------------------------------------------------------------------------------------------
    // Constructors / Destructors

    CastPosition();
    CastPosition(int id, QString position);
    CastPosition(CastPosition *position);

    ~CastPosition();

    //------------------------------------------------------------------------------------------------------------------------------
    // Getters / Setters

    int getId() const;
    void setId(int value);

    QString getPosition() const;
    void setPosition(const QString &value);

private:

    //------------------------------------------------------------------------------------------------------------------------------
    // Variables

    int id;
    QString position;
};

#endif // CASTPOSITION_H
