#include "monitoring.h"
#include <QFileInfo>
#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>

QString Monitoring::getFileInfo(const QString& path)
{
    QFileInfo file(path);
    QString result;
    QTextStream out(&result);

    if (!file.exists()) {
        qDebug() << "Не существует:" << path;
        return result;
    }

    qDebug() << "Расположение: " << file.absolutePath() << "\n";
    qDebug() << "Существует: " << file.exists() << "\n";
    qDebug() << "Изменён: " << file.lastModified().toString() << "\n";
    qDebug() << "Размер: " << file.size() << " байт\n";

    return result;
}

Monitoring::Monitoring(QObject* parent)
    : QObject(parent)
{
    connect(&watcher, &QFileSystemWatcher::fileChanged, this, &Monitoring::onFileChanged);
}

void Monitoring::addFile(const QString& path)
{
    QFileInfo file(path);

    if (!file.exists()) {
        return;
    }

    if (!monitoredFiles.contains(path)) {
        if (watcher.addPath(path)) {
            monitoredFiles.append(path);
        }
    }
}

void Monitoring::removeFile(const QString& path)
{
    monitoredFiles.removeAll(path);
    watcher.removePath(path);
}

void Monitoring::onFileChanged(const QString& path)
{
    QFileInfo info(path);

    if (info.exists()) {
        emit fileModified(path);
        watcher.addPath(path);
    } else {
        monitoredFiles.removeAll(path);
        emit fileDeleted(path);
    }
}

void Monitoring::listFiles()
{
    qDebug() << "\nСписок отслеживаемых файлов:";

    if (monitoredFiles.isEmpty()) {
        qDebug() << "No monitored files";
    } else {
        for (int i = 0; i < monitoredFiles.size(); ++i) {
            QFileInfo file(monitoredFiles[i]);
            qDebug() << QString("%1. %2 (size: %3 bytes)")
                            .arg(i + 1)
                            .arg(monitoredFiles[i])
                            .arg(file.exists() ? file.size() : 0);
        }
    }
    qDebug() << "Total files:" << monitoredFiles.size() << "\n";
}

void Monitoring::showStatus(const QString& path)
{
    QFileInfo file(path);
    qDebug() << "\nFile information:";
    qDebug() << "Path:" << path;

    if (!file.exists()) {
        qDebug() << "File does not exist";
    } else {
        qDebug() << "File exists";
        qDebug() << "Size:" << file.size() << "bytes";
        qDebug() << "Last modified:" << file.lastModified().toString(Qt::ISODate);
        qDebug() << "Location:" << file.absolutePath();
        qDebug() << "Monitored:" << (monitoredFiles.contains(path) ? "Yes" : "No");
    }
    qDebug() << "";
}

void Monitoring::clearAll()
{
    int count = monitoredFiles.size();
    for (const QString& file : monitoredFiles) {
        watcher.removePath(file);
    }
    monitoredFiles.clear();
    qDebug() << "\nУдалено" << count << "файлов из мониторинга\n";
}

void Monitoring::showHelp()
{
    qDebug() << "\nСправка по командам мониторинга:";
    qDebug() << "  add <path>      - добавить файл или папку в мониторинг";
    qDebug() << "  remove <path>   - удалить файл или папку из мониторинга";
    qDebug() << "  list            - показать все отслеживаемые файлы";
    qDebug() << "  status <path>   - показать информацию о файле";
    qDebug() << "  clear           - удалить все файлы из мониторинга";
    qDebug() << "  help            - показать справку по командам";
    qDebug() << "  exit            - выйти из программы\n";
}
