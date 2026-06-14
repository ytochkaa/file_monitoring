#ifndef DIRECTORYWALKER_H
#define DIRECTORYWALKER_H

#include <QString>
#include <QVector>

/**
 * @brief Класс для рекурсивного обхода директорий.
 */
class DirectoryWalker
{
public:
    /**
     * @brief Рекурсивно возвращает список всех файлов в директории.
     * @param path путь к директории.
     * @return список абсолютных путей к файлам.
     */
    QVector<QString> listFilesRecursively(const QString& path);
};

#endif
