#ifndef MONITORING_H
#define MONITORING_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QSet>
#include <QHash>
#include <QDateTime>
#include "pollingtimer.h"

class ILogger;
class CommandReader;

struct FileState
{
    QDateTime lastModified;
    long int size;
};

/**
 * @brief Класс для отслеживания изменений файлов.
 *
 * Использует QFileSystemWatcher и таймер опроса для обнаружения
 * изменений существования и размера наблюдаемых файлов.
 */
class Monitoring : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param logger     логгер для вывода событий.
     * @param intervalMs интервал опроса таймера в миллисекундах.
     * @param parent     родительский объект Qt.
     */
    explicit Monitoring(ILogger& logger, int intervalMs = 100, QObject* parent = nullptr);

    /**
     * @brief Подключает сигналы CommandReader к слотам Monitoring.
     * @param reader источник команд пользователя.
     */
    void connectTo(CommandReader& reader);

public slots:
    /**
     * @brief Добавляет файл или директорию в мониторинг.
     *
     * Если путь указывает на директорию — добавляются все файлы рекурсивно.
     * @param path путь к файлу или директории.
     */
    void addFile(const QString& path);

    /**
     * @brief Удаляет файл или директорию из мониторинга.
     * @param path путь к файлу или директории.
     */
    void removeFile(const QString& path);
    void listFiles();
    void showHelp();

signals:
    /**
     * @brief Сигнал: файл добавлен в мониторинг.
     * @param path путь к файлу.
     */
    void fileAdded(const QString& path);

    /**
     * @brief Сигнал: наблюдаемый файл был изменён.
     * @param path путь к файлу.
     */
    void fileModified(const QString& path);

    /**
     * @brief Сигнал: наблюдаемый файл был удалён с диска.
     * @param path путь к файлу.
     */
    void fileDeleted(const QString& path);

private slots:
    void onFileChanged(const QString& path);
    void onTimerTick();

private:
    QFileSystemWatcher watcher;
    QSet<QString> monitoredFiles;
    QHash<QString, FileState> fileStates;
    PollingTimer poller;
    ILogger& logger;
};

#endif
