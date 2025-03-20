#ifndef CUSTOMLINEEDIT_H
#define CUSTOMLINEEDIT_H

#include <QObject>
#include <QLineEdit>

class CustomLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    CustomLineEdit(QWidget *parent = 0);
     ~CustomLineEdit();

signals:
  void focussed(bool hasFocus);
  void pressed();

protected:
  virtual void focusInEvent(QFocusEvent *e);
  virtual void focusOutEvent(QFocusEvent *e);
  virtual void mousePressEvent(QMouseEvent *event);
};

#endif // CUSTOMLINEEDIT_H
