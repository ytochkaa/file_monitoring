#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>

class Monitoring : public QObject
{
    Q_OBJECT

public:
    QString getFileInfo(const QString& path);
    explicit Monitoring(QObject* parent = nullptr);
    void addFile(const QString& path);
    void removeFile(const QString& path);

signals:
    void fileModified(const QString& path);
    void fileDeleted(const QString& path);

private slots:
    void onFileChanged(const QString& path);

private:
    QFileSystemWatcher watcher;
    QStringList monitoredFiles;
};