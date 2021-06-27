#include "fscannerworker.h"

#include <QMutexLocker>

FScannerWorker::FScannerWorker(QObject* parent, const QDir& dir, bool recursive, FScannerData& scannerData, QMutex& mutex) :
    QThread(parent),
    m_recursive(recursive),
    m_rootDir(dir),
    m_scannerData(scannerData),
    m_mutex(mutex)
{

}

void FScannerWorker::run()
{
    QMutexLocker locker(&m_mutex);
    scanDir(m_rootDir, m_recursive);

    m_scannerData.extensionList = generateExtensionList(m_scannerData.extensions);
    emit done();
}

QList<ExtensionPair> FScannerWorker::generateExtensionList(const QHash<QString, size_t>& extensions) const
{
    QList<ExtensionPair> list;
    list.reserve(extensions.count());

    QHashIterator it(extensions);
    while (it.hasNext()) {
        it.next();
        list.emplaceBack(it.key(), it.value());
    }

    std::sort(list.begin(), list.end(), [](ExtensionPair l, ExtensionPair r) {
        return l.second > r.second;
    });

    return list;
}

void FScannerWorker::scanDir(const QDir& dir, bool recursive)
{
    auto fileList = dir.entryInfoList(QDir::Files | QDir::Writable, QDir::Name);
    for (const auto& fileInfo : fileList) {
        m_scannerData.targets.push_back(fileInfo);

        auto extension = fileInfo.suffix().toLower();
        if (m_scannerData.extensions.find(extension) == m_scannerData.extensions.end()) {
            // no key
            m_scannerData.extensions.insert(extension, 1);
        } else {
            // increment count
            m_scannerData.extensions[extension] += 1;
        }
    }

    if (fileList.count() > 0)
        emit countUpdated(m_scannerData.targets.count());

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
