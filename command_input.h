#ifndef COMMAND_INPUT_H
#define COMMAND_INPUT_H

#include <QThread>
#include <QString>

/**
 * @brief Читает команды пользователя из stdin в отдельном потоке.
 */
class CommandReader : public QThread
{
    Q_OBJECT

public:
    explicit CommandReader(QObject* parent = nullptr);

signals:
    void addRequested(const QString& path);
    void removeRequested(const QString& path);
    void exitRequested();

protected:
    void run() override;
};

#endif
