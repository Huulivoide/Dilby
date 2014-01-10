#include <QUrl>
#include <QNetworkRequest>
#include <QTemporaryFile>
#include <QDebug>
#include <QEventLoop>

#include "downloader.h"

Downloader::Downloader(QObject *parent) : QObject(parent)
{
  connect(&manager, SIGNAL(finished(QNetworkReply*)), this,
          SLOT(downloadFinished(QNetworkReply*)));
}

void Downloader::setFile(const QString &f)
{
  file.setFileName(f);
}

void Downloader::setUrl(const QString &u)
{
  url.setUrl(u);
}

void Downloader::download()
{
  QEventLoop loop;
  connect(this, SIGNAL(done()), &loop, SLOT(quit()));

  QNetworkRequest request(url);

  qDebug() << tr("Requesting url: %1").arg(url.toString());

  connect(manager.get(request), SIGNAL(downloadProgress(qint64,qint64)), this,
          SLOT(downloadProgress(qint64,qint64)));

  loop.exec();
}

void Downloader::downloadFinished(QNetworkReply *data)
{
  if (!file.open(QIODevice::WriteOnly))
  {
      qDebug() << "";
      return;
  }

  const QByteArray sdata = data->readAll();
  file.write(sdata);
  qDebug() << tr("Data written to file: %1").arg(file.fileName());
  file.close();

  emit done();
}

void Downloader::downloadProgress(qint64 recieved, qint64 total)
{
  double percentage = 0.0;
  if (total > 0)
    percentage = (double)recieved / (double)total * 100.0;

  qDebug() << tr("%1%: %2 bytes of %3 bytes transferred").arg(percentage, 3, 'f', 0)
              .arg(recieved, 6)
              .arg(total, 6);
}
