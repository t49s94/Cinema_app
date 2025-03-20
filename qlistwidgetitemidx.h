#ifndef QLISTWIDGETITEMIDX_H
#define QLISTWIDGETITEMIDX_H

#include <QListWidgetItem>

class QListWidgetItemIdx : public QListWidgetItem
{
public:
    QListWidgetItemIdx();

    int getIndex() const;
    void setIndex(int value);

private:
    int index;
};

#endif // QLISTWIDGETITEMIDX_H
