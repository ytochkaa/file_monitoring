#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>
#include <QDateTime>

/**
 * @brief Возвращает текущее время в формате ISO 8601.
 * @return Строка вида "ГГГГ-ММ-ДДTЧЧ:ММ:СС".
 */
inline QString makeTimestamp()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

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
    virtual void logAdded(const QString& path, qint64 size) = 0;

    /**
     * @brief Вызывается при удалении файла из мониторинга.
     * @param path путь к файлу.
     */
    virtual void logRemoved(const QString& path) = 0;

    /**
     * @brief Вызывается при изменении наблюдаемого файла.
     * @param path путь к файлу.
     * @param size новый размер файла в байтах.
     */
    virtual void logModified(const QString& path, qint64 size) = 0;

    /**
     * @brief вызывается при удалении наблюдаемого файла с диска.
     * @param path путь к файлу.
     */
    virtual void logDeleted(const QString& path) = 0;
};

#endif
