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

bool FScanner::isValid()
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

size_t FScanner::getFileCount()
{
    QMutexLocker locker(&m_mutex);
    return m_scannerData.targets.count();
}

QList<ExtensionPair> FScanner::getExtensionList()
{
    QMutexLocker locker(&m_mutex);

    QList<ExtensionPair> list;
    list.reserve(m_scannerData.extensions.count());

    QHashIterator it(m_scannerData.extensions);
    while (it.hasNext()) {
        it.next();
        list.emplaceBack(it.key(), it.value());
    }

    std::sort(list.begin(), list.end(), [](ExtensionPair l, ExtensionPair r) {
        return l.second > r.second;
    });

    return list;
}

QPair<QMutex&, const QList<QFileInfo>&> FScanner::getFileList()
{
    return { m_mutex, m_scannerData.targets };
}

void FScanner::setFilteredIndices(const QSet<size_t>& indices)
{
    QMutexLocker locker(&m_mutex);
    m_scannerData.filteredIndices = indices;
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
