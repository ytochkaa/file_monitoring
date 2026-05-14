#include "FileLogger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

FileLogger::FileLogger(const QString& logFilePath)
    : logFilePath(logFilePath)
{
}

static QString makeTimestamp()
{
    return QDateTime::currentDateTime().toString(Qt::ISODate);
}

void FileLogger::writeLog(const QString& message)
{
    QFile file(logFilePath);
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        return;
    }

    QTextStream out(&file);
    out << makeTimestamp() << " " << message << Qt::endl;
}

void FileLogger::logAdded(const QString& path)
{
    writeLog("[ADDED] " + path);
}

void FileLogger::logRemoved(const QString& path)
{
    writeLog("[REMOVED] " + path);
}

void FileLogger::logModified(const QString& path)
{
    writeLog("[MODIFIED] " + path);
}

void FileLogger::logDeleted(const QString& path)
{
    writeLog("[DELETED] " + path);
}
