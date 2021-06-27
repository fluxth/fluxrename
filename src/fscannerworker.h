#ifndef FSCANNERWORKER_H
#define FSCANNERWORKER_H

#include <QThread>
#include <QDir>

#include "fscanner.h"

class FScannerWorker : public QThread
{
    Q_OBJECT
public:
    explicit FScannerWorker(QObject* parent, const QDir& dir, bool recursive, FScannerData& scannerData, QMutex& mutex);
    void run() override;

private:
    bool m_recursive;
    QDir m_rootDir;

    FScannerData& m_scannerData;
    QMutex& m_mutex;

    void scanDir(const QDir& dir, bool recursive = false);

    QList<ExtensionPair> generateExtensionList(const QHash<QString, size_t>& extensions) const;

signals:
    void countUpdated(size_t count);
    void done();

};

#endif // FSCANNERWORKER_H
