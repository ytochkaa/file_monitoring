#ifndef DIRECTORYWALKER_H
#define DIRECTORYWALKER_H

#include <QString>
#include <QStringList>
#include <QTextStream>

class DirectoryWalker
{
public:
    void listDirsRecursively(const QString& path, QTextStream& out);
    QStringList listFilesRecursively(const QString& path);
};

#endif
