#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>

struct FileState
{
    QString path;
    QDateTime lastModified;
    int size;
};

class Monitoring : public QObject
{
    Q_OBJECT

public:
    QString getFileInfo(const QString& path);
    explicit Monitoring(QObject* parent = nullptr);

public slots:
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
    QMap<QString, FileState> fileStates;
};
