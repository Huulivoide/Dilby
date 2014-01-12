#include <QDate>
#include <QRegExp>
#include <QPixmap>
#include <QFile>
#include <QDir>

#include "dilbert.h"
#include "downloader.h"

QDate Dilbert::min(QDate(1989, 4, 16));
QDate Dilbert::max(QDate::currentDate());

Dilbert::Dilbert(const QString &cd, QObject *parent) : Scraper(parent), cacheDir(cd)
{
}

QString Dilbert::getComic(const QDate &date, int quality) const
{
  QString dString(date.toString("yyyy-MM-dd"));
  QString fileName = cacheDir + "Dilbert-" + dString + prefix(quality) + ".gif";

  if (QFile::exists(fileName))
    return fileName;

  Downloader dl;
  dl.setFile(QDir::tempPath() + "/Dilbert-" + dString + ".html");
  dl.setUrl("http://dilbert.com/strips/comic/" + dString);
  dl.download();

  QFile html(dl.fileName());
  if (!html.open(QIODevice::ReadOnly))
  {
    emit error(tr("Cannot open the html file for scraping."));
    return "";
  }
  QString data(html.readAll());
  html.remove();

  QRegExp re(regexString(quality));
  re.indexIn(data);
  QString imgUrl = re.cap(0);

  if (imgUrl.isEmpty())
  {
    emit error(tr("No image url could be scrapped."));
    return "";
  }

  dl.setUrl("http://dilbert.com" + imgUrl);
  dl.setFile(fileName);
  dl.download();

  QPixmap test;
  if (!test.load(fileName))
  {
    emit error(tr("Error loading the image file: %1\nPlease try again.").arg(fileName));
    QFile(fileName).remove(); // Make sure we don't leave invalid file in the cache
    return "";
  }

  return fileName;
}


QDate Dilbert::next(const QDate &current) const
{
  // Dilbert is released daily
  if (current.addDays(1) <= max)
    return current.addDays(1);

  return max;
}

QDate Dilbert::previous(const QDate &current) const
{
  // Dilbert is released daily
  if (current.addDays(-1) >= min)
    return current.addDays(-1);

  return min;
}


QString Dilbert::prefix(int quality) const
{
  if (quality == 0)
    return ".LOW";
  else if (quality == 1)
    return ".HG";

  return "";
}

QString Dilbert::regexString(int quality) const
{
  if (quality == 1)
    return "(\\/[^\\s]*\\.zoom\\.gif)";

  return "(\\/[^\\s]*\\.print\\.gif)";
}
