#include "ConsoleLogger.h"
#include <QDebug>

void ConsoleLogger::logAdded(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[ADDED]" << path;
}

void ConsoleLogger::logRemoved(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[REMOVED]" << path;
}

void ConsoleLogger::logModified(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[MODIFIED]" << path;
}

void ConsoleLogger::logDeleted(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[DELETED]" << path;
}
