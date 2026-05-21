#include <QCoreApplication>
#include <QDebug>

#include "command_input.h"
#include "ConsoleLogger.h"
#include "monitoring.h"

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "Запуск программы";

    ConsoleLogger logger;
    Monitoring monitor(&logger);
    CommandReader reader;

    qDebug() << "Мониторинг запущен";

    QObject::connect(&reader, &CommandReader::addRequested, &monitor, &Monitoring::addFile);
    QObject::connect(&reader, &CommandReader::removeRequested, &monitor, &Monitoring::removeFile);
    QObject::connect(&reader, &CommandReader::listRequested, &monitor, &Monitoring::listFiles);
    QObject::connect(&reader, &CommandReader::statusRequested, &monitor, &Monitoring::showStatus);
    QObject::connect(&reader, &CommandReader::clearRequested, &monitor, &Monitoring::clearAll);
    QObject::connect(&reader, &CommandReader::helpRequested, &monitor, &Monitoring::showHelp);
    QObject::connect(&reader, &CommandReader::exitRequested, &a, &QCoreApplication::quit);

    reader.start();

    int result = a.exec();
    reader.wait();

    return result;
}
