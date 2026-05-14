#ifndef ILOGGER_H
#define ILOGGER_H

#include <QString>

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