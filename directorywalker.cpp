#include "directorywalker.h"

#include <QDir>
#include <QFileInfo>

QVector<QString> DirectoryWalker::listFilesRecursively(const QString& path)
{
    QDir dir(path);
    QVector<QString> files;

    if (!dir.exists()) {
        return files;
    }

    dir.setFilter(QDir::Files | QDir::NoSymLinks);
    for (const QString& fileName : dir.entryList()) {
        QFileInfo file(dir.absoluteFilePath(fileName));
        const QString canonicalPath = file.canonicalFilePath();
        files.append(canonicalPath.isEmpty() ? file.absoluteFilePath() : canonicalPath);
    }

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (const QString& subDir : dir.entryList()) {
        files.append(listFilesRecursively(dir.absoluteFilePath(subDir)));
    }

    return files;
}
