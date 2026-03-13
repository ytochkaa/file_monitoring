#include "Monitoring.h"
#include <QFileInfo>

Monitoring::Monitoring(QObject* parent)
    : QObject(parent)
{
    connect(&watcher, &QFileSystemWatcher::fileChanged,
            this, &Monitoring::onFileChanged);
}

void Monitoring::addFile(const QString& path)
{
    if (!monitoredFiles.contains(path)) {
        monitoredFiles.append(path);
        watcher.addPath(path);
    }
}

void Monitoring::removeFile(const QString& path)
{
    monitoredFiles.removeAll(path);
    watcher.removePath(path);
}

void Monitoring::onFileChanged(const QString& path)
{
    QFileInfo info(path);

    if (info.exists()) {
        emit fileModified(path);
        watcher.addPath(path);
    } else {
        emit fileDeleted(path);
    }
}