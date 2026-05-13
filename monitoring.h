#ifndef MONITORING_H
#define MONITORING_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QSet>
#include <QHash>
#include <QDateTime>
#include <QTimer>

class ILogger;

struct FileState
{
    QDateTime lastModified;
    long int size;
};

class Monitoring : public QObject
{
    Q_OBJECT

public:
    QString getFileInfo(const QString& path);
    explicit Monitoring(ILogger* logger = nullptr, QObject* parent = nullptr);

public slots:
    void addFile(const QString& path);
    void removeFile(const QString& path);

signals:
    void fileAdded(const QString& path);
    void fileModified(const QString& path);
    void fileDeleted(const QString& path);

private slots:
    void onFileChanged(const QString& path);
    void onTimerTick();

private:
    QFileSystemWatcher watcher;
    QSet<QString> monitoredFiles;
    QHash<QString, FileState> fileStates;
    QTimer* timer;
    ILogger* logger;
};

#endif
