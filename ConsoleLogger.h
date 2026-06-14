#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ILogger.h"

/**
 * @brief Реализация ILogger, выводящая события в консоль через std::cout.
 */
class ConsoleLogger : public ILogger
{
public:
    /**
     * @brief Выводит сообщение о добавлении файла с его размером.
     * @param path путь к файлу.
     * @param size размер файла в байтах.
     */
    void logAdded(const QString& path, long int size) override;

    /**
     * @brief Выводит сообщение об удалении файла из мониторинга с его последним размером.
     * @param path путь к файлу.
     * @param size последний известный размер файла в байтах.
     */
    void logRemoved(const QString& path, long int size) override;

    /**
     * @brief Выводит сообщение об изменении файла с его новым размером.
     * @param path путь к файлу.
     * @param size новый размер файла в байтах.
     */
    void logModified(const QString& path, long int size) override;

    /**
     * @brief Выводит сообщение об удалении файла с диска с его последним размером.
     * @param path путь к файлу.
     * @param size последний известный размер файла в байтах.
     */
    void logDeleted(const QString& path, long int size) override;

    /**
     * @brief Выводит сообщение об ошибке.
     * @param message описание ошибки.
     */
    void logError(const QString& message) override;

    /**
     * @brief Выводит ответ на команду пользователя без отметки времени и тега.
     * @param message текст для вывода.
     */
    void logInfo(const QString& message) override;

private:
    /**
     * @brief Выводит событие файла с отметкой времени, тегом, путём и размером.
     * @param tag  тег события, например [ADDED] или [MODIFIED].
     * @param path путь к файлу или текст сообщения.
     * @param size размер файла в байтах; при -1 строка размера не выводится.
     */
    void logFileEvent(const QString& tag, const QString& path, long int size = -1);
};

#endif
