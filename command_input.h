#ifndef COMMAND_INPUT_H
#define COMMAND_INPUT_H

#include <QThread>
#include <QString>

class ILogger;

/**
 * @brief Читает команды пользователя из stdin в отдельном потоке.
 */
class CommandReader : public QThread
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор.
     * @param logger логгер для вывода подсказок и ошибок ввода. Если nullptr — вывод отключён.
     * @param parent родительский объект Qt.
     */
    explicit CommandReader(ILogger* logger = nullptr, QObject* parent = nullptr);

signals:
    void addRequested(const QString& path);
    void removeRequested(const QString& path);
    void listRequested();
    void helpRequested();
    void exitRequested();

protected:
    void run() override;

private:
    ILogger* logger;
};

#endif
