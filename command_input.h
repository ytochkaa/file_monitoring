#ifndef COMMAND_INPUT_H
#define COMMAND_INPUT_H

#include <QDebug>
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
    void listRequested();
    void statusRequested(const QString& path);
    void clearRequested();
    void helpRequested();
    void exitRequested();

protected:
    void run() override
    {
        QTextStream in(stdin);

        while (true) {
            qDebug() << "\nВведите команду:";
            qDebug() << "  add <path>      - добавить файл или папку в мониторинг";
            qDebug() << "  remove <path>   - удалить файл или папку из мониторинга";
            qDebug() << "  list            - показать все отслеживаемые файлы";
            qDebug() << "  status <path>   - показать информацию о файле";
            qDebug() << "  clear           - удалить все файлы из мониторинга";
            qDebug() << "  help            - показать справку по командам";
            qDebug() << "  exit            - выйти из программы\n";
            QString line = in.readLine().trimmed();

            if (line.isEmpty()) {
                qDebug() << "Empty input, please enter a command.";
                continue;
            }

            QStringList parts = line.split(' ', Qt::SkipEmptyParts);

            if (parts.isEmpty()) {
                continue;
            }

            QString command = parts[0].toLower();

            if (command == "add") {
                if (parts.size() < 2) {
                    qDebug() << "Error: specify a path. Example: add C:/files/test.txt";
                    continue;
                }
                QString filePath = line.section(' ', 1);
                emit addRequested(filePath);
                qDebug() << "Add request:" << filePath;
            } else if (command == "remove") {
                if (parts.size() < 2) {
                    qDebug() << "Error: specify a path. Example: remove C:/files/test.txt";
                    continue;
                }
                QString filePath = line.section(' ', 1);
                emit removeRequested(filePath);
                qDebug() << "Remove request:" << filePath;
            } else if (command == "list") {
                emit listRequested();
            } else if (command == "status") {
                if (parts.size() < 2) {
                    qDebug() << "Error: specify a path. Example: status C:/files/test.txt";
                    continue;
                }
                QString filePath = line.section(' ', 1);
                emit statusRequested(filePath);
            } else if (command == "clear") {
                emit clearRequested();
            } else if (command == "help") {
                emit helpRequested();
            } else if (command == "exit") {
                qDebug() << "Exiting application...";
                emit exitRequested();
                break;
            } else {
                qDebug() << "Unknown command:" << command << ". Type 'help' for available commands.";
            }
        }
    }
};

#endif
