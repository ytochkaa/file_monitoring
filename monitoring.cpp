#include "monitoring.h"
#include <QFileInfo>
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

QString Monitoring::getFileInfo(const QString& path)
{
    QFileInfo file(path);
    QString result;
    QTextStream out(&result);

    if (!file.exists()) {
        qDebug() << "Не существует:" << path;
        return result;
    }

    qDebug() << "Расположение: " << file.absolutePath() << "\n";
    qDebug() << "Существует: " << file.exists() << "\n";
    qDebug() << "Изменён: " << file.lastModified().toString() << "\n";
    qDebug() << "Размер: " << file.size() << " байт\n";

    return result;
}

Monitoring::Monitoring(QObject* parent)
    : QObject(parent)
{
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Monitoring::onFileChanged);
}

void Monitoring::addFile(const QString& path)
{
    QFileInfo file(path);

    if (!file.exists()) {
        return;
    }

    if (!monitoredFiles.contains(path)) {
        if (watcher.addPath(path)) {
            monitoredFiles.append(path);
        }
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
        monitoredFiles.removeAll(path);
        emit fileDeleted(path);
    }
}
