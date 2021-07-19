#ifndef FRENAMER_H
#define FRENAMER_H

#include <QObject>

#include "global.h"
#include "fscanner.h"
#include "models/frenameitem.h"

class FRenamer : public QObject
{
    Q_OBJECT
public:
    explicit FRenamer(unique_ptr<FScanner> scanner, QWidget* parent);
    ~FRenamer();

    bool calculate();
    bool previewChanges();

    void commitRename();

    unique_ptr<FScanner> takeScanner();

private:
    QWidget* m_parent;
    unique_ptr<FScanner> m_scanner;
    QList<FRenameItem> m_renameItems;

    bool processItem(const QFileInfo& fileInfo);

    optional<QString> processFilename(const QFileInfo& fileInfo);
    inline bool processFilenameReplace(const RenameConfig::ReplaceConfig& config, QString& filename);
    inline bool processFilenameTrim(const RenameConfig::TrimConfig& config, QString& filename);
    inline bool processFilenameAdd(const RenameConfig::AddConfig& config, QString& filename);
    inline bool processFilenameStringManipulation(const RenameConfig::StringManipulationConfig& config, QString& filename);

signals:

};

#endif // FRENAMER_H
