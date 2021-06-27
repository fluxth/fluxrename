#ifndef FRENAMEITEM_H
#define FRENAMEITEM_H

#include "../global.h"

#include <QString>
#include <QFileInfo>

struct FRenameItem {
    QFileInfo fileInfo;
    QString newFilename;

    FRenameItem() = default;
};

#endif // FRENAMEITEM_H
