#include "logger.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

Logger::Logger()
{
}

// добавить время когда был изменён и info(размер и т.д.)
void Logger::logModified(const QString& path)
{
    qDebug() << "ИЗМЕНЁН:" << path;
}

void Logger::logDeleted(const QString& path)
{
    qDebug() << "УДАЛЁН:" << path;
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
