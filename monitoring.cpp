#include "monitoring.h"
#include <QFileInfo>
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

static FileState makeFileState(const QString& path)
{
    QFileInfo file(path);
    FileState state;
    state.path = path;
    state.lastModified = file.lastModified();
    state.size = file.exists() ? static_cast<int>(file.size()) : 0;
    return state;
}

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
    , timer(new QTimer(this))
{
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Monitoring::onFileChanged);
    connect(timer, &QTimer::timeout, this, &Monitoring::onTimerTick);
    timer->start(500);
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
            fileStates[path] = makeFileState(path);
        }
    }
}

void Monitoring::removeFile(const QString& path)
{
    monitoredFiles.removeAll(path);
    watcher.removePath(path);
    fileStates.remove(path);
}

void Monitoring::onFileChanged(const QString& path)
{
    QFileInfo info(path);

    if (info.exists()) {
        fileStates[path] = makeFileState(path);
        emit fileModified(path);
        watcher.addPath(path);
    } else {
        monitoredFiles.removeAll(path);
        fileStates.remove(path);
        emit fileDeleted(path);
    }
}

void Monitoring::onTimerTick()
{
    auto it = fileStates.begin();
    while (it != fileStates.end()) {
        const QString path = it.key();
        QFileInfo info(path);

        if (!info.exists()) {
            emit fileDeleted(path);
            monitoredFiles.removeAll(path);
            it = fileStates.erase(it);
            continue;
        }

        if (info.lastModified() != it->lastModified || info.size() != it->size) {
            emit fileModified(path);
            it.value() = makeFileState(path);
        }

        ++it;
    }
}
