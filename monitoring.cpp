#include "monitoring.h"
#include "command_input.h"
#include "ILogger.h"
#include "directorywalker.h"
#include "pollingtimer.h"
#include <QFileInfo>
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

void Monitoring::connectTo(CommandReader& reader)
{
    connect(&reader, &CommandReader::addRequested, this, &Monitoring::addFile);
    connect(&reader, &CommandReader::removeRequested, this, &Monitoring::removeFile);
    connect(&reader, &CommandReader::listRequested, this, &Monitoring::listFiles);
    connect(&reader, &CommandReader::helpRequested, this, &Monitoring::showHelp);
}

Monitoring::Monitoring(ILogger& logger, int intervalMs, QObject* parent)
    : QObject(parent)
    , poller(intervalMs)
    , logger(logger)
{
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Monitoring::onFileChanged);
    connect(&poller, &PollingTimer::tick, this, &Monitoring::onTimerTick);
}

void Monitoring::addFile(const QString& path)
{
    const QString normalizedInput = normalizePath(path);
    QFileInfo file(normalizedInput);

    if (!file.exists()) {
        logger.logError("Нельзя добавить несуществующий путь: " + path);
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
            logger.logError("Не удалось добавить путь в watcher: " + filePath);
            return;
        }
        monitoredFiles.insert(filePath);
        const FileState state = makeFileState(filePath);
        fileStates[filePath] = state;
        emit fileAdded(filePath);
        logger.logAdded(filePath, state.size);
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
            const long int size = fileStates.value(filePath).size;
            monitoredFiles.remove(filePath);
            watcher.removePath(filePath);
            fileStates.remove(filePath);
            logger.logRemoved(filePath, size);
        }

        return;
    }

    const QString filePath = file.exists() ? normalizePath(file) : normalizedInput;
    if (monitoredFiles.contains(filePath)) {
        const long int size = fileStates.value(filePath).size;
        monitoredFiles.remove(filePath);
        watcher.removePath(filePath);
        fileStates.remove(filePath);
        logger.logRemoved(filePath, size);
    }
}

void Monitoring::onFileChanged(const QString& path)
{
    const QString normalized = normalizePath(path);
    QFileInfo info(normalized);

    if (info.exists()) {
        const FileState state = makeFileState(normalized);
        fileStates[normalized] = state;
        emit fileModified(normalized);
        logger.logModified(normalized, state.size);
        if (!watcher.files().contains(normalized) && !watcher.addPath(normalized)) {
            logger.logError("Не удалось повторно добавить путь в watcher: " + normalized);
        }
    } else {
        const long int size = fileStates.value(normalized).size;
        watcher.removePath(normalized);
        monitoredFiles.remove(normalized);
        fileStates.remove(normalized);
        emit fileDeleted(normalized);
        logger.logDeleted(normalized, size);
    }
}

void Monitoring::onTimerTick()
{
    auto it = fileStates.begin();
    while (it != fileStates.end()) {
        const QString path = it.key();
        QFileInfo info(path);

        if (!info.exists()) {
            const long int size = it->size;
            watcher.removePath(path);
            monitoredFiles.remove(path);
            it = fileStates.erase(it);
            emit fileDeleted(path);
            logger.logDeleted(path, size);
            continue;
        }

        if (info.lastModified() != it->lastModified || info.size() != it->size) {
            it.value() = makeFileState(path);
            emit fileModified(path);
            logger.logModified(path, it->size);
        }

        ++it;
    }
}

void Monitoring::listFiles()
{
    if (monitoredFiles.isEmpty()) {
        logger.logInfo("Список отслеживаемых файлов пуст.");
        return;
    }

    logger.logInfo("Отслеживаемые файлы:");
    int index = 1;
    for (const QString& filePath : qAsConst(monitoredFiles)) {
        QFileInfo file(filePath);
        const long int size = file.exists() ? static_cast<long int>(file.size()) : 0;
        logger.logInfo(QString("  %1. %2 — %3 байт").arg(index++).arg(filePath).arg(size));
    }
    logger.logInfo(QString("Всего: %1").arg(monitoredFiles.size()));
}

void Monitoring::showHelp()
{
    logger.logInfo("Справка по командам мониторинга:");
    logger.logInfo("  add <путь>     - добавить файл или папку в мониторинг");
    logger.logInfo("  remove <путь>  - удалить файл или папку из мониторинга");
    logger.logInfo("  list           - показать все отслеживаемые файлы");
    logger.logInfo("  help           - показать справку по командам");
    logger.logInfo("  exit           - выйти из программы");
}
