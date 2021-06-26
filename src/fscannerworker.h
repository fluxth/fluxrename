#ifndef FSCANNERWORKER_H
#define FSCANNERWORKER_H

#include <QThread>
#include <QDir>

class FScannerWorker : public QThread
{
    Q_OBJECT
public:
    explicit FScannerWorker(QObject* parent, const QDir& dir, bool recursive, QList<QFileInfo>* targets, QMutex& mutex);
    void run() override;

private:
    bool m_recursive;
    QDir m_rootDir;
    QList<QFileInfo>* m_targets;
    QMutex& m_mutex;

    void scanDir(const QDir& dir, bool recursive = false);

signals:
    void countUpdated(size_t count);
    void done();

};

#endif // FSCANNERWORKER_H
