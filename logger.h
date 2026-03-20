#include <QString>

class Logger
{
public:
    Logger(const QString& logFilePath);
    void logModified(const QString& path);
    void logDeleted(const QString& path);
};
