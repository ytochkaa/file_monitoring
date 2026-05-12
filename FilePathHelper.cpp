#include "FilePathHelper.h"
#include <QDir>

QString FilePathHelper::normalizePath(const QString& path)
{
    QFileInfo file(path);
    return normalizePath(file);
}

QString FilePathHelper::normalizePath(const QFileInfo& file)
{
    const QString canonicalPath = file.canonicalFilePath();
    const QString rawPath = canonicalPath.isEmpty() ? file.absoluteFilePath() : canonicalPath;
    return QDir::cleanPath(QDir::fromNativeSeparators(rawPath));
}

QFileInfo FilePathHelper::normalizedFileInfo(const QString& path)
{
    return QFileInfo(normalizePath(path));
}
