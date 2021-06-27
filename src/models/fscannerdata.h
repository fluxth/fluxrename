#ifndef FSCANNERDATA_H
#define FSCANNERDATA_H

#include <QFileInfo>

#import "../global.h"
#import "frenameconfig.h"

typedef QPair<QString, size_t> ExtensionPair;

struct FScannerData {
    QList<QFileInfo> targets;
    QHash<QString, size_t> extensions;
    QList<ExtensionPair> extensionList;
    QSet<size_t> filteredIndices;
    FRenameConfig renameConfig;

    FScannerData() = default;
};

#endif // FSCANNERDATA_H
