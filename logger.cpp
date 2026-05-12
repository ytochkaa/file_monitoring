#include "logger.h"
#include <QDateTime>
#include <QDebug>

Logger::Logger()
{
}

void Logger::logModified(const QString& path)
{
    qDebug().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate)
                       << "[MODIFIED]" << path;
}

void Logger::logDeleted(const QString& path)
{
    qDebug().noquote() << QDateTime::currentDateTime().toString(Qt::ISODate)
                       << "[DELETED]" << path;
}
