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
    void exitRequested();

protected:
    void run() override
    {
        QTextStream in(stdin);

        while (true) {
            qDebug() << "Введите команду (добавить <путь>, удалить <путь>, выход):";
            QString line = in.readLine().trimmed();

            if (line.isEmpty()) {
                qDebug() << "Пусто! Введите команду.";
                continue;
            }

            QString lowerLine = line.toLower();

            if (lowerLine == "exit" || lowerLine == "выход") {
                qDebug() << "Выход из программы...";
                emit exitRequested();
                break;
            }

            QStringList parts = line.split(' ', Qt::SkipEmptyParts);

            if (parts.size() < 2) {
                qDebug() << "Ошибка: добавить <путь> или удалить <путь>";
                continue;
            }

            QString command = parts[0].toLower();
            QString filePath = line.section(' ', 1);

            if (command == "add" || command == "добавить") {
                emit addRequested(filePath);
                qDebug() << "Запрос на добавление файла:" << filePath;
            } else if (command == "del" || command == "удалить") {
                emit removeRequested(filePath);
                qDebug() << "Запрос на удаление файла:" << filePath;
            } else {
                qDebug() << "Неизвестная команда:" << command;
            }
        }
    }
};

#endif
