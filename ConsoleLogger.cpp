#include "ConsoleLogger.h"
#include <QDebug>

void ConsoleLogger::logAdded(const QString& path, qint64 size)
{
    qDebug().noquote() << makeTimestamp() << "[ADDED]" << path;
    qDebug().noquote() << "  Существует: да";
    qDebug().noquote() << "  Размер:" << size << "байт";
}

void ConsoleLogger::logRemoved(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[REMOVED]" << path;
}

void ConsoleLogger::logModified(const QString& path, qint64 size)
{
    qDebug().noquote() << makeTimestamp() << "[MODIFIED]" << path;
    qDebug().noquote() << "  Существует: да";
    qDebug().noquote() << "  Размер:" << size << "байт";
}

void ConsoleLogger::logDeleted(const QString& path)
{
    qDebug().noquote() << makeTimestamp() << "[DELETED]" << path;
}
