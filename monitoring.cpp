#include "monitoring.h"
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

Monitoring::Monitoring(ILogger* logger, int intervalMs, QObject* parent)
    : QObject(parent)
    , poller(new PollingTimer(intervalMs, this))
    , logger(logger)
{
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Monitoring::onFileChanged);
    connect(poller, &PollingTimer::tick, this, &Monitoring::onTimerTick);
}

void Monitoring::addFile(const QString& path)
{
    const QString normalizedInput = normalizePath(path);
    QFileInfo file(normalizedInput);

    if (!file.exists()) {
        if (logger) {
            logger->logError("Нельзя добавить несуществующий путь: " + path);
        }
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
            if (logger) {
                logger->logError("Не удалось добавить путь в watcher: " + filePath);
            }
            return;
        }
        monitoredFiles.insert(filePath);
        const FileState state = makeFileState(filePath);
        fileStates[filePath] = state;
        emit fileAdded(filePath);
        if (logger) {
            logger->logAdded(filePath, state.size);
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
        const FileState state = makeFileState(normalized);
        fileStates[normalized] = state;
        emit fileModified(normalized);
        if (logger) {
            logger->logModified(normalized, state.size);
        }
        if (!watcher.files().contains(normalized) && !watcher.addPath(normalized)) {
            if (logger) {
                logger->logError("Не удалось повторно добавить путь в watcher: " + normalized);
            }
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

void Monitoring::listFiles()
{
    qDebug() << "\nСписок отслеживаемых файлов:";

    if (monitoredFiles.isEmpty()) {
        qDebug() << "No monitored files";
    } else {
        int index = 1;
        for (const QString& filePath : qAsConst(monitoredFiles)) {
            QFileInfo file(filePath);
            qDebug() << QString("%1. %2 (size: %3 bytes)")
                            .arg(index++)
                            .arg(filePath)
                            .arg(file.exists() ? file.size() : 0);
        }
    }
    qDebug() << "Total files:" << monitoredFiles.size() << "\n";
}

void Monitoring::showStatus(const QString& path)
{
    const QString normalizedPath = FilePathHelper::normalizePath(path);
    QFileInfo file(normalizedPath);
    qDebug() << "\nFile information:";
    qDebug() << "Path:" << normalizedPath;

    if (!file.exists()) {
        qDebug() << "File does not exist";
    } else {
        qDebug() << "File exists";
        qDebug() << "Size:" << file.size() << "bytes";
        qDebug() << "Last modified:" << file.lastModified().toString(Qt::ISODate);
        qDebug() << "Location:" << file.absolutePath();
        qDebug() << "Monitored:" << (monitoredFiles.contains(normalizedPath) ? "Yes" : "No");
    }
    qDebug() << "";
}

void Monitoring::clearAll()
{
    int count = monitoredFiles.size();
    for (const QString& file : monitoredFiles) {
        watcher.removePath(file);
    }
    monitoredFiles.clear();
    fileStates.clear();
    qDebug() << "\nУдалено" << count << "файлов из мониторинга\n";
}

void Monitoring::showHelp()
{
    qDebug() << "\nСправка по командам мониторинга:";
    qDebug() << "  add <path>      - добавить файл или папку в мониторинг";
    qDebug() << "  remove <path>   - удалить файл или папку из мониторинга";
    qDebug() << "  list            - показать все отслеживаемые файлы";
    qDebug() << "  status <path>   - показать информацию о файле";
    qDebug() << "  clear           - удалить все файлы из мониторинга";
    qDebug() << "  help            - показать справку по командам";
    qDebug() << "  exit            - выйти из программы\n";
}
