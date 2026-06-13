#ifndef DIRECTORYWALKER_H
#define DIRECTORYWALKER_H

#include <QString>
#include <QVector>
#include <QTextStream>

/**
 * @brief Класс для рекурсивного обхода директорий.
 */
class DirectoryWalker
{
public:
    /**
     * @brief Рекурсивно выводит все поддиректории в поток.
     * @param path путь к корневой директории.
     * @param out  поток вывода.
     */
    void listDirsRecursively(const QString& path, QTextStream& out);

    /**
     * @brief Рекурсивно возвращает список всех файлов в директории.
     * @param path путь к директории.
     * @return список абсолютных путей к файлам.
     */
    QVector<QString> listFilesRecursively(const QString& path);
};

#endif
