#include "qlistwidgetitemidx.h"

QListWidgetItemIdx::QListWidgetItemIdx()
{

}

int QListWidgetItemIdx::getIndex() const
{
    return index;
}

void QListWidgetItemIdx::setIndex(int value)
{
    index = value;
}
