#include "monitoring.h"
#include "directorywalker.h"
#include <QFileInfo>
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>

static FileState makeFileState(const QString& path)
{
    QFileInfo file(path);
    FileState state;
    state.lastModified = file.lastModified();
    state.size = file.exists() ? static_cast<long int>(file.size()) : 0;
    return state;
}

static QString normalizePath(const QFileInfo& file)
{
    const QString canonicalPath = file.canonicalFilePath();
    const QString rawPath = canonicalPath.isEmpty() ? file.absoluteFilePath() : canonicalPath;
    return QDir::cleanPath(QDir::fromNativeSeparators(rawPath));
}

static QString normalizePath(const QString& path)
{
    return normalizePath(QFileInfo(path));
}

QString Monitoring::getFileInfo(const QString& path)
{
    QFileInfo file(normalizePath(path));
    QString result;
    QTextStream out(&result);

    if (!file.exists()) {
        out << "Не существует: " << path;
        return result;
    }

    out << "Расположение: " << file.absolutePath() << '\n';
    out << "Существует: " << (file.exists() ? "да" : "нет") << '\n';
    out << "Изменён: " << file.lastModified().toString(Qt::ISODate) << '\n';
    out << "Размер: " << file.size() << " байт\n";

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
    const QString normalizedInput = normalizePath(path);
    QFileInfo file(normalizedInput);

    if (!file.exists()) {
        return;
    }

    if (file.isDir()) {
        DirectoryWalker walker;
        const QStringList files = walker.listFilesRecursively(path);

        for (const QString& filePath : files) {
            addFile(filePath);
        }

        return;
    }

    const QString filePath = normalizePath(file);

    if (!monitoredFiles.contains(filePath)) {
        if (!watcher.addPath(filePath)) {
            qWarning() << "Не удалось добавить путь в watcher:" << filePath;
            return;
        }
        monitoredFiles.append(filePath);
        fileStates[filePath] = makeFileState(filePath);
    }
}

void Monitoring::removeFile(const QString& path)
{
    const QString normalizedInput = normalizePath(path);
    QFileInfo file(normalizedInput);

    if (file.exists() && file.isDir()) {
        QString dirPath = normalizePath(file);

        if (!dirPath.endsWith('/')) {
            dirPath += '/';
        }

        const QStringList files = monitoredFiles;
        for (const QString& filePath : files) {
            if (filePath.startsWith(dirPath)) {
                monitoredFiles.removeAll(filePath);
                watcher.removePath(filePath);
                fileStates.remove(filePath);
            }
        }

        return;
    }

    const QString filePath = file.exists() ? normalizePath(file) : normalizedInput;
    monitoredFiles.removeAll(filePath);
    watcher.removePath(filePath);
    fileStates.remove(filePath);
}

void Monitoring::onFileChanged(const QString& path)
{
    const QString normalized = normalizePath(path);
    QFileInfo info(normalized);

    if (info.exists()) {
        fileStates[normalized] = makeFileState(normalized);
        emit fileModified(normalized);
        if (!watcher.files().contains(normalized) && !watcher.addPath(normalized)) {
            qWarning() << "Не удалось повторно добавить путь в watcher:" << normalized;
        }
    } else {
        monitoredFiles.removeAll(normalized);
        fileStates.remove(normalized);
        emit fileDeleted(normalized);
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
