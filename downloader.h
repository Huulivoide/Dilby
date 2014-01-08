#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrl>
#include <QFile>

class Downloader : public QObject
{
    Q_OBJECT
  public:
    explicit Downloader(QObject *parent = 0);

    void setFile(const QString &f);
    void setUrl(const QString &u);
    QString fileName() { return file.fileName(); };

  signals:
    void done();

  public slots:
    void download();
    void downloadFinished(QNetworkReply *data);
    void downloadProgress(qint64 recieved, qint64 total);

  private:
    QNetworkAccessManager manager;
    QUrl url;
    QFile file;
};

#endif // DOWNLOADER_H
