#ifndef FSCANNER_H
#define FSCANNER_H

#import <QObject>
#import <QString>
#import <QFileInfo>
#import <QDir>

class FScanner : public QObject
{
    Q_OBJECT
public:
    FScanner(const QString& path, bool recursive = false);
    ~FScanner();

    bool isRecursive() const;
    bool isValid() const;
    bool exists() const;

    QString getAbsolutePath() const;
    size_t getFileCount() const;

    void scan();

private:
    QDir m_rootDir;
    bool m_recursive;

    QList<QFileInfo> m_targets;
    QMutex m_mutex;

    void scanDir(const QDir& dir, bool recursive = false);

signals:
    void countUpdated(size_t count);
    void done();
};

#endif // FSCANNER_H
