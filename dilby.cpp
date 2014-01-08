#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QRegExp>

#include <QMessageBox>

#include <QGraphicsPixmapItem>
#include <QMovie>

#include "dilby.h"
#include "ui_dilby.h"
#include "downloader.h"

Dilby::Dilby(QWidget *parent) : QMainWindow(parent), ui(new Ui::Dilby), settings("", "Dilby")
{
  ui->setupUi(this);

  baseDir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0) + "/";

  QDir bd;
  if (!bd.mkpath(baseDir))
  {
      qDebug() << "Cannot create the data dir: " + baseDir;
      qApp->quit();
  }

  ui->comicView->setScene(&comic);
  comic.addItem(&pixmap);

  QMovie *loader = new QMovie(":/res/icons/loader.gif", QByteArray(), ui->loaderLabel);
  ui->loaderLabel->hide();
  ui->loaderLabel->setMovie(loader);

  ui->comicDate->setMaximumDate(QDate::currentDate());
  if (settings.contains("currentDate"))
    ui->comicDate->setDate(settings.value("currentDate").toDate());
  else // If requested date is "bigger" than the newest one,  the site returns the newest one
    ui->comicDate->setDate(QDate::currentDate());

  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(save()));
}

Dilby::~Dilby()
{
  delete ui;
}

QString Dilby::getComic(const QDate &date)
{
  QString dString(date.toString("yyyy-MM-dd"));
  QString fileName = baseDir + dString + ".gif";

  if (QFile::exists(fileName))
    return fileName;

  Downloader dl;
  dl.setFile(QDir::tempPath() + "/Dilbert-" + dString + ".html");
  dl.setUrl("http://dilbert.com/strips/comic/" + dString);
  dl.download();

  QFile html(dl.fileName());
  if (!html.open(QIODevice::ReadOnly))
  {
    qDebug() << tr("Cannot open the html file.");
    return "";
  }

  QString data(html.readAll());
  html.remove();

  QRegExp re("(\\/[^\\s]*\\.zoom\\.gif)");
  if (re.indexIn(data) < 0)
    return "";

  QString imgUrl = re.cap(0);
  if (imgUrl.isEmpty())
    return "";

  dl.setUrl("http://dilbert.com" + imgUrl);
  dl.setFile(fileName);
  dl.download();

  QPixmap test;
  if (!test.load(fileName))
  {
    qDebug() << tr("Error loading the image file: %1").arg(fileName);
    return "";
  }

  return fileName;
}

void Dilby::setComic(const QDate &date)
{
  ui->comicView->hide();
  ui->loaderLabel->show();
  ui->loaderLabel->movie()->start();

  QString fileName = getComic(date);

  ui->loaderLabel->movie()->stop();
  ui->loaderLabel->hide();
  ui->comicView->show();


  if (!fileName.isEmpty())
  {
    currentDate = date;

    QPixmap imgData(fileName);
    pixmap.setPixmap(imgData);

    ui->comicView->setFixedHeight(imgData.height());
    ui->comicView->setFixedWidth(imgData.width());
    ui->centralWidget->updateGeometry();
    QSize size = this->sizeHint();
    this->setFixedSize(size);
  }
  else // It triggers a new signal to rerun this function but.......
    ui->comicDate->setDate(currentDate);
}

void Dilby::save()
{
  settings.setValue("currentDate", currentDate);
}

void Dilby::on_action_Quit_triggered()
{
  qApp->quit();
}

void Dilby::on_comicDate_dateChanged(const QDate &date)
{
  setComic(date);
}

void Dilby::on_buttonPrevious_clicked()
{
  QDate current = ui->comicDate->date();
  if (current.addDays(-1) >= ui->comicDate->minimumDate())
  {
    QDate d = ui->comicDate->date();
    ui->comicDate->setDate(d.addDays(-1));
  }
}

void Dilby::on_buttonNext_clicked()
{
  QDate current = ui->comicDate->date();
  if (current.addDays(1) <= ui->comicDate->maximumDate())
  {
    QDate d = ui->comicDate->date();
    ui->comicDate->setDate(d.addDays(1));
  }
}

void Dilby::on_action_About_triggered()
{
  QMessageBox::about(this, tr("About Dilby"),
                     tr("<h1>Dilby</h1>The source code can be found at:<br>%1<br> To contact send email to:<br>%2")
                     .arg("<a href=\"http://foo.bar\">http://foo.bar</a>")
                     .arg("<a href=\"mailto:jesse.jaara@gmail.com\">jesse.jaara@gmail.com</a>"));
}

void Dilby::on_actionAbout_QT_triggered()
{
    QMessageBox::aboutQt(this);
}
