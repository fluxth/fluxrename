#include "fscannerworker.h"

#include <QMutexLocker>

FScannerWorker::FScannerWorker(QObject* parent, const QDir& dir, bool recursive, QList<QFileInfo>* targets, QMutex& mutex) :
    QThread(parent),
    m_recursive(recursive),
    m_rootDir(dir),
    m_targets(targets),
    m_mutex(mutex)
{

}

void FScannerWorker::run()
{
    if (m_targets == nullptr)
        return;

    QMutexLocker locker(&m_mutex);
    scanDir(m_rootDir, m_recursive);
    emit done();
}

void FScannerWorker::scanDir(const QDir& dir, bool recursive)
{
    auto fileList = dir.entryInfoList(QDir::Files | QDir::Writable, QDir::Name);
    for (const auto& fileInfo : fileList) {
        m_targets->push_back(fileInfo);
    }

    if (fileList.count() > 0)
        emit countUpdated(m_targets->count());

    if (recursive) {
        auto dirList = dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot | QDir::Writable, QDir::Name);
        for (const auto& fileInfo : dirList) {
            if (fileInfo.isDir()) {
                auto subDir = QDir(fileInfo.absoluteFilePath());
                scanDir(subDir, true);
            }
        }
    }

    return;
}
