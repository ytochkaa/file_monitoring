#include <QCoreApplication>
#include <QDebug>
#include <QThread>

#include "command_input.h"
#include "logger.h"
#include "monitoring.h"

bool isRunning = true;

void stopMonitoring()
{
    isRunning = false;
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    while (true) {
        qDebug() << "Запуск программы";

        Monitoring monitor;
        CommandReader reader;

        qDebug() << "Мониторинг запущен";

        QObject::connect(&reader, &CommandReader::addRequested, &monitor, &Monitoring::addFile);

        QObject::connect(&reader, &CommandReader::removeRequested, &monitor, &Monitoring::removeFile);

        QObject::connect(&reader, &CommandReader::exitRequested, stopMonitoring);

        QObject::connect(&monitor, &Monitoring::fileModified, fileWasModified);

        QObject::connect(&monitor, &Monitoring::fileDeleted, fileWasDeleted);

        reader.start();

        while (true) {
            a.processEvents();

            if (!isRunning) {
                break;
            }

            QThread::msleep(100);
        }

        reader.wait();
        break;
    }

    return 0;
}
