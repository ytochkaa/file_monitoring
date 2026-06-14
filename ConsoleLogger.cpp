#include "ConsoleLogger.h"
#include <QDateTime>
#include <iostream>

static QString makeTimestamp()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

void ConsoleLogger::logFileEvent(const QString& tag, const QString& path, long int size)
{
    std::cout << makeTimestamp().toStdString() << " " << tag.toStdString() << " " << path.toStdString() << "\n";
    if (size >= 0) {
        std::cout << "  Размер: " << size << " байт\n";
    }
    std::cout.flush();
}

void ConsoleLogger::logAdded(const QString& path, long int size)
{
    logFileEvent("[ADDED]", path, size);
}

void ConsoleLogger::logRemoved(const QString& path, long int size)
{
    logFileEvent("[REMOVED]", path, size);
}

void ConsoleLogger::logModified(const QString& path, long int size)
{
    logFileEvent("[MODIFIED]", path, size);
}

void ConsoleLogger::logDeleted(const QString& path, long int size)
{
    logFileEvent("[DELETED]", path, size);
}

void ConsoleLogger::logError(const QString& message)
{
    logFileEvent("[ERROR]", message);
}

void ConsoleLogger::logInfo(const QString& message)
{
    std::cout << message.toStdString() << "\n";
    std::cout.flush();
}
