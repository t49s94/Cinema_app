#include "customlineedit.h"

CustomLineEdit::CustomLineEdit(QWidget *parent)
 : QLineEdit(parent)
{}

CustomLineEdit::~CustomLineEdit()
{}

void CustomLineEdit::focusInEvent(QFocusEvent *e)
{
  QLineEdit::focusInEvent(e);
  emit(focussed(true));
}

void CustomLineEdit::focusOutEvent(QFocusEvent *e)
{
  QLineEdit::focusOutEvent(e);
  emit(focussed(false));
}

void CustomLineEdit::mousePressEvent(QMouseEvent *event)
{
    QLineEdit::mousePressEvent(event);
    emit(pressed());
}
