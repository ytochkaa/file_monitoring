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

void fileWasModified(const QString& path)
{
    qDebug() << "Файл изменён:" << path;
    Logger logger;
    logger.logModified(path);
}

void fileWasDeleted(const QString& path)
{
    qDebug() << "Файл удалён:" << path;
    Logger logger;
    logger.logDeleted(path);
}
