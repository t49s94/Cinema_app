/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class inherites from QLineEdit. It's a custom QLineEdit that provides focusInEvent and focusOutEvent.

*/

#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <qmainwindow.h>

#include <QLineEdit>

class MyLineEdit : public QLineEdit
{
    Q_OBJECT

    public:

      //----------------------------------------------------------------------------------------------------------------------------
      // Constructors / Destructor

      MyLineEdit(QWidget *parent = 0);
      ~MyLineEdit();

    signals:

      void focussed(bool hasFocus);

    protected:

      //----------------------------------------------------------------------------------------------------------------------------
      // Slots

      // Emit signal focussed with @param bool true that means that focusInEvent was handled
      // @param QFocusEvent *e.
      virtual void focusInEvent(QFocusEvent *e);
      // Emit signal focussed with @param bool false that means that focusOutEvent was handled
      // @param QFocusEvent *e.
      virtual void focusOutEvent(QFocusEvent *e);
};

#endif // MYLINEEDIT_H
