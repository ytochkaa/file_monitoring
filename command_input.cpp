#include "command_input.h"
#include <QTextStream>
#include <QStringList>

CommandReader::CommandReader(QObject* parent)
    : QThread(parent)
{
}

void CommandReader::run()
{
    QTextStream in(stdin);
    QTextStream out(stdout);

    while (true) {
        out << "Введите команду (add <путь>, remove <путь>, exit): " << Qt::flush;
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

        if (command == "add" || command == "remove") {
            if (parts.size() < 2) {
                out << "Ошибка: укажите путь. Пример: add C:/files/test.txt\n"
                    << Qt::flush;
                continue;
            }
            QString filePath = line.section(' ', 1);
            if (command == "add") {
                emit addRequested(filePath);
            } else {
                emit removeRequested(filePath);
            }
        } else {
            out << "Неизвестная команда: " << command << "\n"
                << Qt::flush;
        }
    }
}
