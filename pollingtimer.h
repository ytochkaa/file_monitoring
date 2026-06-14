#ifndef POLLINGTIMER_H
#define POLLINGTIMER_H

#include <QObject>
#include <QTimer>

/**
 * @brief Периодически испускает сигнал tick() через заданный интервал.
 */
class PollingTimer : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief Конструктор. Запускает таймер сразу при создании.
     * @param intervalMs интервал между сигналами в миллисекундах.
     * @param parent     родительский объект Qt.
     */
    explicit PollingTimer(int intervalMs = 100, QObject* parent = nullptr);

signals:
    /**
     * @brief Сигнал: наступил момент для опроса файлов.
     */
    void tick();

private:
    QTimer timer;
};

#endif
