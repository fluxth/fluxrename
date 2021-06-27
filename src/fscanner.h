#ifndef FSCANNER_H
#define FSCANNER_H

#import <QObject>
#import <QString>
#import <QFileInfo>
#import <QDir>

#import "global.h"
#import "models/fscannerdata.h"

class FScanner : public QObject
{
    Q_OBJECT
public:
    friend class FRenamer;

    explicit FScanner(const QString& path, bool recursive = false);
    ~FScanner();

    bool isRecursive() const;
    bool isValid() const;
    bool exists() const;

    QString getAbsolutePath() const;
    size_t getFileCount() const;

    QPair<QMutex&, const QList<ExtensionPair>&> getExtensionList() const;
    QPair<QMutex&, const QList<QFileInfo>&> getFileList() const;
    QPair<QMutex&, const FRenameConfig&> getRenameConfig() const;
    QPair<QMutex&, FRenameConfig&> getMutRenameConfig();

    void setFilteredIndices(const QSet<size_t>& indices);

    QPair<QMutex&, const FScannerData&> getData() const;

    void scan();

private:
    QDir m_rootDir;
    bool m_recursive;

    FScannerData m_scannerData;
    mutable QMutex m_mutex;

    void scanDir(const QDir& dir, bool recursive = false);

signals:
    void countUpdated(size_t count);
    void done();
};

#endif // FSCANNER_H
