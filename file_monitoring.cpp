#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>

#include "Monitoring.h"
// QFileSystemWatcher

void itFile(const QString& path, QTextStream& out)
{
    QFileInfo file(path);

    if (!file.exists()) {
        out << "Не существует:" << path << Qt::endl;
        return;
    }
    out << "Расположение:" << file.absolutePath() << Qt::endl;

    qDebug() << "Существует:" << file.exists();
    qDebug() << "Изменён:" << file.lastModified().toString();
    qDebug() << "Размер:" << file.size() << "байт";
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Development of information security tools/Сompil_cmake.txt";
    // QString startPath = "C:/Users/darya/Desktop/Combez/7_semester/Number_Theory_Methods_in_Cryptography";

    QTextStream out(stdout);

    itFile(startPath, out);

    Monitoring monitor;

    monitor.addFile(startPath);

    // Изменениефайла
    QObject::connect(&monitor, &Monitoring::fileModified, [&](const QString& path) {
        qDebug() << "File modified:" << path;
        itFile(path, out);
    });

    // Удаление файла
    QObject::connect(&monitor, &Monitoring::fileDeleted, [&](const QString& path) {
        qDebug() << "File deleted:" << path;
    });

    return 0;
}
