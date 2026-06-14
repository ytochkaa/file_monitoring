#include "command_input.h"
#include "ILogger.h"
#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>

CommandReader::CommandReader(ILogger* logger, QObject* parent)
    : QThread(parent)
    , logger(logger)
{
    connect(this, &CommandReader::exitRequested,
            QCoreApplication::instance(), &QCoreApplication::quit);
}

void CommandReader::run()
{
    QTextStream in(stdin);

    if (logger) {
        logger->logInfo("Монитор файлов запущен. Команды: add <путь>, remove <путь>, list, help, exit");
    }

    while (true) {
        QString line = in.readLine().trimmed();

        if (line.isEmpty()) {
            continue;
        }

        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        QString command = parts[0].toLower();

        if (command == "exit") {
            emit exitRequested();
            break;
        }

        if (command == "list") {
            emit listRequested();
        } else if (command == "help") {
            emit helpRequested();
        } else if (command == "add" || command == "remove") {
            if (parts.size() < 2) {
                if (logger) {
                    logger->logError("Укажите путь. Пример: add C:/files/test.txt");
                }
                continue;
            }
            QString filePath = line.section(' ', 1);
            if (command == "add") {
                emit addRequested(filePath);
            } else {
                emit removeRequested(filePath);
            }
        } else {
            if (logger) {
                logger->logError("Неизвестная команда: " + command);
            }
        }
    }
}
