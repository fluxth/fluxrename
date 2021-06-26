#include "fscanner.h"

#include "fscannerworker.h"

FScanner::FScanner(const QString& path, bool recursive) :
    QObject(),
    m_rootDir(path),
    m_recursive(recursive),
    m_targets(),
    m_mutex()
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
    return exists() && m_targets.count() > 0;
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
    return m_targets.count();
}

void FScanner::scan()
{
    if (exists()) {
        FScannerWorker* worker = new FScannerWorker(this, m_rootDir, m_recursive, &m_targets, m_mutex);
        connect(worker, &FScannerWorker::countUpdated, this, &FScanner::countUpdated);
        connect(worker, &FScannerWorker::done, this, &FScanner::done);
        worker->start();
    } else {
        emit done();
    }
}
