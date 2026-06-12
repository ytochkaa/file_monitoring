#include "directorywalker.h"

#include <QDir>
#include <QFileInfo>

void DirectoryWalker::listDirsRecursively(const QString& path, QTextStream& out)
{
    QDir dir(path);

    if (!dir.exists()) {
        out << "Путь не существует: " << path << Qt::endl;
        return;
    }

    out << "Директория: " << dir.absolutePath() << Qt::endl;

    dir.setFilter(QDir::Dirs | QDir::NoDotAndDotDot | QDir::NoSymLinks);
    for (const QString& subDir : dir.entryList()) {
        listDirsRecursively(dir.absoluteFilePath(subDir), out);
    }
}

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
