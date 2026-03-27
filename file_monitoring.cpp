#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>

#include "Monitoring.h"
#include "logger.h"
// QFileSystemWatcher

void itFile(const QString& path, QTextStream& out)
{
    QFileInfo file(path);

    if (!file.exists()) {
        qDebug() << "Не существует:" << path;
        return;
    }
    qDebug() << "Расположение:" << file.absolutePath();

    qDebug() << "Существует:" << file.exists();
    qDebug() << "Изменён:" << file.lastModified().toString();
    qDebug() << "Размер:" << file.size() << "байт";
}

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "ВКЛЮЧИЛА СЛЕЖКУ";

    // QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Development of information security tools/Сompil_cmake.txt";
    // QString startPath = "C:/Users/darya/Desktop/Combez/7_semester/Number_Theory_Methods_in_Cryptography";
    QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Programming technologies/file_monitoring/file_monitoring/tests/test_case1.txt";
    // QString startPath = "C:\\Users\\darya\\Desktop\\Combez\\8_semester\\Programming technologies\\file_monitoring\\file_monitoring\\tests\\test_case1.txt";

    QTextStream out(stdout);

    itFile(startPath, out);

    Monitoring monitor;

    monitor.addFile(startPath);

    // Изменение файла
    QObject::connect(&monitor, &Monitoring::fileModified, [&](const QString& path) {
        qDebug() << "Файл изменён" << path;
        // itFile(path, out);
        Logger logger;
        // itFile(path, out);
        logger.logModified(path);
    });

    // Удаление файла
    QObject::connect(&monitor, &Monitoring::fileDeleted, [&](const QString& path) {
        qDebug() << "Файл удалён:" << path;
        Logger logger;
        // itFile(path, out);
        logger.logDeleted(path);
    });

    // return 0;
    return a.exec();
}