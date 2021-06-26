#ifndef FSCANNER_H
#define FSCANNER_H

#import <QObject>
#import <QString>
#import <QFileInfo>
#import <QDir>

struct FScannerData {
    QList<QFileInfo> targets;
    QHash<QString, size_t> extensions;
    QSet<size_t> filteredIndices;
};

typedef QPair<QString, size_t> ExtensionPair;

class FScanner : public QObject
{
    Q_OBJECT
public:
    FScanner(const QString& path, bool recursive = false);
    ~FScanner();

    bool isRecursive() const;
    bool isValid();
    bool exists() const;

    QString getAbsolutePath() const;
    size_t getFileCount();

    QList<ExtensionPair> getExtensionList();
    QPair<QMutex&, const QList<QFileInfo>&> getFileList();

    void setFilteredIndices(const QSet<size_t>& indices);

    void scan();

private:
    QDir m_rootDir;
    bool m_recursive;

    FScannerData m_scannerData;
    QMutex m_mutex;

    void scanDir(const QDir& dir, bool recursive = false);

signals:
    void countUpdated(size_t count);
    void done();
};

#endif // FSCANNER_H
