#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

Logger::Logger(const QString& logFilePath)
    : logFile(logFilePath)
{
}
// добавить время когда был изменён и info(размер и т.д.)
void Logger::logModified(const QString& path)
{
    qDebug() << "MODIFIED: " << path;
}

void Logger::logDeleted(const QString& path)
{
    qDebug() << "DELETED: " << path;
}