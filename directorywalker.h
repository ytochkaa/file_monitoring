#ifndef DIRECTORYWALKER_H
#define DIRECTORYWALKER_H

#include <QString>
#include <QVector>
#include <QTextStream>

class DirectoryWalker
{
public:
    void listDirsRecursively(const QString& path, QTextStream& out);
    QVector<QString> listFilesRecursively(const QString& path);
};

#endif
