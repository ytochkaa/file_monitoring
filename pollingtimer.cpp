#include "pollingtimer.h"
#include <QTimer>

PollingTimer::PollingTimer(int intervalMs, QObject* parent)
    : QObject(parent)
    , timer(new QTimer(this))
{
    connect(timer, &QTimer::timeout, this, &PollingTimer::tick);
    timer->start(intervalMs);
}
