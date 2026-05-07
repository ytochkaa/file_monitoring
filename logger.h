#ifndef LOGGER_H
#define LOGGER_H

#include <QString>

class Logger
{
public:
    Logger();

    void logModified(const QString& path);
    void logDeleted(const QString& path);
};

#endif
