#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ILogger.h"

/**
 * @brief Реализация ILogger, выводящая события в консоль через qDebug().
 */
class ConsoleLogger : public ILogger
{
public:
    /**
     * @brief Выводит сообщение о добавлении файла с его размером.
     * @param path путь к файлу.
     * @param size размер файла в байтах.
     */
    void logAdded(const QString& path, qint64 size) override;

    /**
     * @brief Выводит сообщение об удалении файла из мониторинга.
     * @param path путь к файлу.
     */
    void logRemoved(const QString& path) override;

    /**
     * @brief Выводит сообщение об изменении файла с его новым размером.
     * @param path путь к файлу.
     * @param size новый размер файла в байтах.
     */
    void logModified(const QString& path, qint64 size) override;

    /**
     * @brief Выводит сообщение об удалении файла с диска.
     * @param path путь к файлу.
     */
    void logDeleted(const QString& path) override;

    /**
     * @brief Выводит сообщение об ошибке.
     * @param message описание ошибки.
     */
    void logError(const QString& message) override;

private:
    /**
     * @brief Выводит событие файла с тегом, путём и размером.
     * @param tag  тег события, например [ADDED] или [MODIFIED].
     * @param path путь к файлу.
     * @param size размер файла в байтах.
     */
    void logFileEvent(const QString& tag, const QString& path, qint64 size);
};

#endif
