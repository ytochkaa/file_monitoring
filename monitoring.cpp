#include "Monitoring.h"
#include <QFileInfo>
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTextStream>
#include <QDateTime>

QString itFile(const QString& path)
{
    QFileInfo file(path);

    QString result;
    QTextStream out(&result);

    if (!file.exists()) {
        out << "Не существует:" << path;
        return result;
    } else {
        out << "Расположение:" << file.absolutePath() << "\n";
        out << "Существует:" << file.exists() << "\n";
        out << "Изменён:" << file.lastModified().toString() << "\n";
        out << "Размер:" << file.size() << "байт" << "\n";

        return result;
    }
}

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