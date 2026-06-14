#include "pollingtimer.h"

PollingTimer::PollingTimer(int intervalMs, QObject* parent)
    : QObject(parent)
{
    connect(&timer, &QTimer::timeout, this, &PollingTimer::tick);
    timer.start(intervalMs);
}
