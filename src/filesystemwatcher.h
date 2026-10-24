#ifndef FILE_SYSTEM_WATCHER_H
#define FILE_SYSTEM_WATCHER_H

#include <QFileSystemWatcher>
#include <QMap>
#include <QMessageBox>
#include <QObject>

class FileSystemWatcher : public QObject {
    Q_OBJECT

public:
    static void addWatchPath(QString path);
    static void removeWatchPath(QString path);

public slots:
    void directoryUpdated(const QString& path); // Called when directory updates; path is monitored path
    void fileUpdated(const QString& path); // Called when file is modified; path is monitored path

private:
    explicit FileSystemWatcher(QObject* parent = 0);

private:
    static FileSystemWatcher* m_pInstance; // Singleton instance
    QFileSystemWatcher* m_pSystemWatcher; // QFileSystemWatcher variable
    QMap<QString, QStringList> m_currentContentsMap; // Current map of monitored directory contents
    bool msgClose = true;
};

#endif // FILE_SYSTEM_WATCHER_H
