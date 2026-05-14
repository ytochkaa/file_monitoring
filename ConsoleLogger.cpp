#include "ConsoleLogger.h"
#include <QDebug>
#include <QDateTime>

static QString makeTimestamp()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

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
