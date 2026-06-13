#ifndef COMMAND_INPUT_H
#define COMMAND_INPUT_H

#include <QTextStream>
#include <QThread>
#include <QStringList>

class CommandReader : public QThread
{
    Q_OBJECT

public:
    explicit CommandReader(QObject* parent = nullptr)
        : QThread(parent)
    {
    }

signals:
    void addRequested(const QString& path);
    void removeRequested(const QString& path);
    void exitRequested();

protected:
    void run() override
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
};

#endif
