#ifndef CONSOLELOGGER_H
#define CONSOLELOGGER_H

#include "ILogger.h"

class ConsoleLogger : public ILogger
{
public:
    void logAdded(const QString& path) override;
    void logRemoved(const QString& path) override;
    void logModified(const QString& path) override;
    void logDeleted(const QString& path) override;
};

#endif