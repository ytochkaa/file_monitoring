#include <QCoreApplication>
#include <QDebug>

#include "command_input.h"
#include "logger.h"
#include "monitoring.h"

int main(int argc, char* argv[])
{
    qDebug() << "Запуск программы";

    QCoreApplication a(argc, argv);

    Monitoring monitor;
    CommandReader reader;

    qDebug() << "Мониторинг запущен";

    QObject::connect(&reader, &CommandReader::addRequested, &monitor, &Monitoring::addFile);

    QObject::connect(&reader, &CommandReader::removeRequested, &monitor, &Monitoring::removeFile);

    QObject::connect(&reader, &CommandReader::exitRequested, &a, &QCoreApplication::quit);

    QObject::connect(&monitor, &Monitoring::fileModified, [&](const QString& path) {
        qDebug() << "Файл изменён:" << path;
        Logger logger;
        logger.logModified(path);
    });

    QObject::connect(&monitor, &Monitoring::fileDeleted, [&](const QString& path) {
        qDebug() << "Файл удалён:" << path;
        Logger logger;
        logger.logDeleted(path);
    });

    reader.start();

    int result = a.exec();
    reader.wait();

    return result;
}
