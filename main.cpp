#include <QCoreApplication>
#include <locale>
#include "command_input.h"
#include "ConsoleLogger.h"
#include "monitoring.h"

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "");
    QCoreApplication a(argc, argv);

    ConsoleLogger logger;
    Monitoring monitor(logger);
    CommandReader reader(logger);

    monitor.connectTo(reader);

    reader.start();
    int result = a.exec();
    reader.wait();
    return result;
}
