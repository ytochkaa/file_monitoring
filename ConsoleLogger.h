#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ILogger.h"

class ConsoleLogger : public ILogger
{
public:
    void logAdded(const QString& path, qint64 size) override;
    void logRemoved(const QString& path) override;
    void logModified(const QString& path, qint64 size) override;
    void logDeleted(const QString& path) override;

private:
    void logFileEvent(const QString& tag, const QString& path, qint64 size);
};

#endif