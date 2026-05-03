#include <QObject>
#include <QFileSystemWatcher>
#include <QStringList>
#include <QMap>
#include <QDateTime>
#include <QTimer>

struct FileState
{
    QDateTime lastModified;
    long int size;
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
    void onTimerTick();

private:
    QFileSystemWatcher watcher;
    QStringList monitoredFiles;
    QMap<QString, FileState> fileStates;
    QTimer* timer;
};
