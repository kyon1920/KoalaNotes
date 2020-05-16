#ifndef MULISTITEMDATA_H
#define MULISTITEMDATA_H

#include <QMetaType>
#include <QString>

typedef struct {
    QString iconPath;
    QString title;
    QString dates;
} MuItemData;

Q_DECLARE_METATYPE(MuItemData)

#endif // MULISTITEMDATA_H
