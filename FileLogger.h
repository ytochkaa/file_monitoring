#ifndef FILELOGGER_H
#define FILELOGGER_H

#include "ILogger.h"
#include <QString>

class FileLogger : public ILogger
{
public:
    explicit FileLogger(const QString& logFilePath);

    void logAdded(const QString& path) override;
    void logRemoved(const QString& path) override;
    void logModified(const QString& path) override;
    void logDeleted(const QString& path) override;

private:
    void writeLog(const QString& message);
    QString logFilePath;
};

#endif