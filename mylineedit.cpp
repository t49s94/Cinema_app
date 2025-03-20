/*

Developer: Sergio Eduardo Santillana Lopez.
Version: 1.2
Last update: 12/16/2020.

This class inherites from QLineEdit. It's a custom QLineEdit that provides focusInEvent and focusOutEvent.

*/

#include "mylineedit.h"

//----------------------------------------------------------------------------------------------------------------------------
// Constructors / Destructor

MyLineEdit::MyLineEdit(QWidget *parent)
 : QLineEdit(parent)
{

}

MyLineEdit::~MyLineEdit()
{

}

//----------------------------------------------------------------------------------------------------------------------------
// Slots

// Emit signal focussed with @param bool true that means that focusInEvent was handled
// @param QFocusEvent *e.
void MyLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);
  emit(focussed(true));
}

// Emit signal focussed with @param bool false that means that focusOutEvent was handled
// @param QFocusEvent *e.
void MyLineEdit::focusOutEvent(QFocusEvent *e)
{
  QLineEdit::focusOutEvent(e);
  emit(focussed(false));
}
