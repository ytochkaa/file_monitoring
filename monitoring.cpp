#include "monitoring.h"
#include "ILogger.h"
#include "directorywalker.h"
#include <QFileInfo>
#include <QDebug>
#include <QDir>

static QString normalizePath(const QFileInfo& file)
{
    const QString canonical = file.canonicalFilePath();
    const QString raw = canonical.isEmpty() ? file.absoluteFilePath() : canonical;
    return QDir::cleanPath(QDir::fromNativeSeparators(raw));
}

static QString normalizePath(const QString& path)
{
    return normalizePath(QFileInfo(path));
}

static FileState makeFileState(const QString& path)
{
    QFileInfo file(path);
    FileState state;
    state.lastModified = file.lastModified();
    state.size = file.exists() ? static_cast<long int>(file.size()) : 0;
    return state;
}

Monitoring::Monitoring(ILogger* logger, QObject* parent)
    : QObject(parent)
    , timer(new QTimer(this))
    , logger(logger)
{
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Monitoring::onFileChanged);
    connect(timer, &QTimer::timeout, this, &Monitoring::onTimerTick);
    timer->start(100);
}

void Monitoring::addFile(const QString& path)
{
    const QString normalizedInput = normalizePath(path);
    QFileInfo file(normalizedInput);

    if (!file.exists()) {
        qWarning() << "Нельзя добавить несуществующий путь:" << path;
        return;
    }

    if (file.isDir()) {
        DirectoryWalker walker;
        const QVector<QString> files = walker.listFilesRecursively(normalizedInput);

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
        monitoredFiles.insert(filePath);
        fileStates[filePath] = makeFileState(filePath);
        emit fileAdded(filePath);
        if (logger) {
            logger->logAdded(filePath, fileStates[filePath].size);
        }
    }
}

void Monitoring::removeFile(const QString& path)
{
    const QString normalizedInput = normalizePath(path);
    QFileInfo file(normalizedInput);

    if (file.exists() && file.isDir()) {
        const QString dirBase = normalizePath(file);
        QString dirPath = dirBase;

        if (!dirPath.endsWith('/')) {
            dirPath += '/';
        }

        QSet<QString> toRemove;
        for (const QString& filePath : qAsConst(monitoredFiles)) {
            if (filePath == dirBase || filePath.startsWith(dirPath)) {
                toRemove.insert(filePath);
            }
        }

        for (const QString& filePath : qAsConst(toRemove)) {
            monitoredFiles.remove(filePath);
            watcher.removePath(filePath);
            fileStates.remove(filePath);
            if (logger) {
                logger->logRemoved(filePath);
            }
        }

        return;
    }

    const QString filePath = file.exists() ? normalizePath(file) : normalizedInput;
    if (monitoredFiles.contains(filePath)) {
        monitoredFiles.remove(filePath);
        watcher.removePath(filePath);
        fileStates.remove(filePath);
        if (logger) {
            logger->logRemoved(filePath);
        }
    }
}

void Monitoring::onFileChanged(const QString& path)
{
    const QString normalized = normalizePath(path);
    QFileInfo info(normalized);

    if (info.exists()) {
        fileStates[normalized] = makeFileState(normalized);
        emit fileModified(normalized);
        if (logger) {
            logger->logModified(normalized, fileStates[normalized].size);
        }
        if (!watcher.files().contains(normalized) && !watcher.addPath(normalized)) {
            qWarning() << "Не удалось повторно добавить путь в watcher:" << normalized;
        }
    } else {
        monitoredFiles.remove(normalized);
        fileStates.remove(normalized);
        emit fileDeleted(normalized);
        if (logger) {
            logger->logDeleted(normalized);
        }
    }
}

void Monitoring::onTimerTick()
{
    auto it = fileStates.begin();
    while (it != fileStates.end()) {
        const QString path = it.key();
        QFileInfo info(path);

        if (!info.exists()) {
            watcher.removePath(path);
            monitoredFiles.remove(path);
            it = fileStates.erase(it);
            emit fileDeleted(path);
            if (logger) {
                logger->logDeleted(path);
            }
            continue;
        }

        if (info.lastModified() != it->lastModified || info.size() != it->size) {
            it.value() = makeFileState(path);
            emit fileModified(path);
            if (logger) {
                logger->logModified(path, it->size);
            }
        }

        ++it;
    }
}
