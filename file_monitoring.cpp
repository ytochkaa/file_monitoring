#include <QCoreApplication>
#include <QDebug>
#include <locale>

#include "command_input.h"
#include "ConsoleLogger.h"
#include "monitoring.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");
    qDebug() << "Запуск программы";

    QCoreApplication a(argc, argv);

    ConsoleLogger logger;
    Monitoring monitor(&logger);
    CommandReader reader;

    qDebug() << "Мониторинг запущен";

    QObject::connect(&reader, &CommandReader::addRequested, &monitor, &Monitoring::addFile);
    QObject::connect(&reader, &CommandReader::removeRequested, &monitor, &Monitoring::removeFile);
    QObject::connect(&reader, &CommandReader::exitRequested, &a, &QCoreApplication::quit);

    reader.start();

    int result = a.exec();
    reader.wait();

    return result;
}
