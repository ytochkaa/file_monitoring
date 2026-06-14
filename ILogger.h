#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>

/**
 * @brief Абстрактный интерфейс логирования событий файлового мониторинга.
 */
class ILogger
{
public:
    virtual ~ILogger() = default;

    /**
     * @brief Вызывается при добавлении файла в мониторинг.
     * @param path путь к файлу.
     * @param size размер файла в байтах.
     */
    virtual void logAdded(const QString& path, long int size) = 0;

    /**
     * @brief Вызывается при удалении файла из мониторинга.
     * @param path путь к файлу.
     * @param size последний известный размер файла в байтах.
     */
    virtual void logRemoved(const QString& path, long int size) = 0;

    /**
     * @brief Вызывается при изменении наблюдаемого файла.
     * @param path путь к файлу.
     * @param size новый размер файла в байтах.
     */
    virtual void logModified(const QString& path, long int size) = 0;

    /**
     * @brief Вызывается при удалении наблюдаемого файла с диска.
     * @param path путь к файлу.
     * @param size последний известный размер файла в байтах.
     */
    virtual void logDeleted(const QString& path, long int size) = 0;

    /**
     * @brief Вызывается при возникновении ошибки.
     * @param message описание ошибки.
     */
    virtual void logError(const QString& message) = 0;

    /**
     * @brief Выводит ответ на команду пользователя (help, list, status, clear).
     *
     * В отличие от событий, печатается без отметки времени и тега —
     * это прямой ответ программы, а не запись в журнал событий.
     * @param message текст для вывода.
     */
    virtual void logInfo(const QString& message) = 0;
};

#endif
