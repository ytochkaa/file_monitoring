#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>
#include <QDateTime>

inline QString makeTimestamp()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

class ILogger
{
public:
    virtual ~ILogger() = default;

    virtual void logAdded(const QString& path) = 0;
    virtual void logRemoved(const QString& path) = 0;
    virtual void logModified(const QString& path) = 0;
    virtual void logDeleted(const QString& path) = 0;
};

#endif