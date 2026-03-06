#include <QDir>
#include <QDebug>
#include <QTextStream>
#include <QCoreApplication>
#include <QFileInfo>
#include <QDateTime>
// QFileSystemWatcher

void itFile(const QString& path, QTextStream& out)
{
    QFileInfo file(path);

    if (!file.exists()) {
        out << "The path does not exist:" << path << Qt::endl;
        return;
    }
    out << "Directory:" << file.absolutePath() << Qt::endl;

    if (file.isFile()) {
        qDebug() << "It is file";
    } else if (file.isDir()) {
        qDebug() << "It is dir";
    }

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

    return 0;
}
