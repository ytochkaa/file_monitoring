#ifndef FILEPATHHELPER_H
#define FILEPATHHELPER_H

#include <QString>
#include <QFileInfo>

class FilePathHelper
{
public:
    static QString normalizePath(const QString& path);
    static QString normalizePath(const QFileInfo& file);
    static QFileInfo normalizedFileInfo(const QString& path);
};

#endif
