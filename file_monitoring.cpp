#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>

#include "monitoring.h"
#include "logger.h"
// QFileSystemWatcher

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    qDebug() << "ВКЛЮЧИЛА СЛЕЖКУ";

    // QString startPath = "...";

    // QTextStream out(stdout);

    // QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Development of information security tools/Сompil_cmake.txt";
    // QString startPath = "C:/Users/darya/Desktop/Combez/7_semester/Number_Theory_Methods_in_Cryptography";
    QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Programming technologies/file_monitoring/file_monitoring/tests/test_case1.txt";
    // QString startPath = "C:\\Users\\darya\\Desktop\\Combez\\8_semester\\Programming technologies\\file_monitoring\\file_monitoring\\tests\\test_case1.txt";

    QTextStream out(stdout);

    Monitoring monitor;

    monitor.addFile(startPath);

    // return 0;
    return a.exec();
}