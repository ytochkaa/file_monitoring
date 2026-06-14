#include "pollingtimer.h"

PollingTimer::PollingTimer(int intervalMs, QObject* parent)
    : QObject(parent)
    , timer(std::make_shared<QTimer>())
{
    connect(timer.get(), &QTimer::timeout, this, &PollingTimer::tick);
    timer->start(intervalMs);
}
