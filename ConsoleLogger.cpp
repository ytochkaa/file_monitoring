#include "ConsoleLogger.h"
#include <QDebug>
#include <QDateTime>

static QString makeTimestamp()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

void ConsoleLogger::logFileEvent(const QString& tag, const QString& path, qint64 size)
{
    qDebug().noquote() << makeTimestamp() << tag << path;
    qDebug().noquote() << "  Размер:" << size << "байт";
}

void ConsoleLogger::logAdded(const QString& path, qint64 size)
{
    logFileEvent("[ADDED]", path, size);
}

void ConsoleLogger::logRemoved(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[REMOVED]" << path;
}

void ConsoleLogger::logModified(const QString& path, qint64 size)
{
    logFileEvent("[MODIFIED]", path, size);
}

void ConsoleLogger::logDeleted(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[DELETED]" << path;
}

void ConsoleLogger::logError(const QString& message)
{
    qDebug().noquote() << makeTimestamp() << "[ERROR]" << message;
}
