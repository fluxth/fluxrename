#include "fscanner.h"

#include "fscannerworker.h"

#include <QMutexLocker>
#include <algorithm>

FScanner::FScanner(const QString& path, bool recursive) :
    QObject(),
    m_rootDir(path),
    m_recursive(recursive),
    m_scannerData()
{

}

FScanner::~FScanner()
{

}

bool FScanner::isRecursive() const
{
    return m_recursive;
}

bool FScanner::isValid() const
{
    QMutexLocker locker(&m_mutex);
    return exists() && m_scannerData.targets.count() > 0;
}

bool FScanner::exists() const
{
    return m_rootDir.exists();
}

QString FScanner::getAbsolutePath() const
{
    return m_rootDir.absolutePath();
}

size_t FScanner::getFileCount() const
{
    QMutexLocker locker(&m_mutex);
    return m_scannerData.targets.count();
}

QPair<QMutex&, const QList<ExtensionPair>&> FScanner::getExtensionList() const
{
    return { m_mutex, m_scannerData.extensionList };
}

QPair<QMutex&, const QList<QFileInfo>&> FScanner::getFileList() const
{
    return { m_mutex, m_scannerData.targets };
}

QPair<QMutex&, const FRenameConfig&> FScanner::getRenameConfig() const
{
    return { m_mutex, m_scannerData.renameConfig };
}

QPair<QMutex&, FRenameConfig&> FScanner::getMutRenameConfig()
{
    return { m_mutex, m_scannerData.renameConfig };
}

void FScanner::setFilteredIndices(const QSet<size_t>& indices)
{
    QMutexLocker locker(&m_mutex);
    m_scannerData.filteredIndices = indices;
}

QPair<QMutex&, const FScannerData&> FScanner::getData() const
{
    return { m_mutex, m_scannerData };
}

void FScanner::scan()
{
    if (exists()) {
        FScannerWorker* worker = new FScannerWorker(this, m_rootDir, m_recursive, m_scannerData, m_mutex);
        connect(worker, &FScannerWorker::countUpdated, this, &FScanner::countUpdated);
        connect(worker, &FScannerWorker::done, this, &FScanner::done);
        worker->start();
    } else {
        emit done();
    }
}
