#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>

#include "monitoring.h"
#include "logger.h"
// QFileSystemWatcher
void сommands(Monitoring& monitor)
{
    QTextStream in(stdin);
    QTextStream out(stdout);

    while (true) {
        out << "Введите команду (add <path>, del <path>, exit): ";
        QString line = in.readLine();

        if (line.isEmpty()) {
            out << "Пусто! Введите команду.\n";
        } else if (line.toLower() == "exit") {
            out << "Выход из программы...\n";
            QCoreApplication::quit();
            break;
        } else {
            QStringList parts = line.split(' ', Qt::SkipEmptyParts);

            if (parts.size() < 2) {
                out << "Ошибка: add <path> или del <path>\n";
            }

            else {
                QString command = parts[0];
                QString filePath = parts[1];

                if (command == "add") {
                    monitor.addFile(filePath);
                    out << "Добавлен файл: " << filePath << "\n";
                } else if (command == "del") {
                    monitor.removeFile(filePath);
                    out << "Удалён файл: " << filePath << "\n";
                } else {
                    out << "Неизвестная команда: " << command << "\n";
                }
            }
        }
    }
}
// QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Development of information security tools/Сompil_cmake.txt";
// QString startPath = "C:/Users/darya/Desktop/Combez/7_semester/Number_Theory_Methods_in_Cryptography";
// QString startPath = "C:/Users/darya/Desktop/Combez/8_semester/Programming technologies/file_monitoring/file_monitoring/tests/test_case1.txt";
// QString startPath = "C:\\Users\\darya\\Desktop\\Combez\\8_semester\\Programming technologies\\file_monitoring\\file_monitoring\\tests\\test_case1.txt";

// добавить list файлов
// принтить больше информации
int main(int argc, char* argv[])
{
    qDebug() << "in main\n";

    QCoreApplication a(argc, argv);

    Monitoring monitor;
    QTextStream out(stdout);

    out << " out Мониторинг запущен\n"
        << Qt::endl;
    qDebug() << " qDebug Мониторинг запущен\n";

    QObject::connect(&monitor, &Monitoring::fileModified, [&](const QString& path) {
        out << "Файл изменён: " << path << "\n";
        Logger logger;
        logger.logModified(path);
    });

    QObject::connect(&monitor, &Monitoring::fileDeleted, [&](const QString& path) {
        out << "Файл удалён: " << path << "\n";
        Logger logger;
        logger.logDeleted(path);
    });

    QTimer::singleShot(0, [&]() {
        сommands(monitor);
    });

    return a.exec();
}