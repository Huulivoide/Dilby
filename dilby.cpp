#include <QDir>
#include <QStandardPaths>
#include <QDebug>
#include <QRegExp>

#include <QMessageBox>

#include <QGraphicsPixmapItem>
#include <QMovie>
#include <QIcon>

#include "dilby.h"
#include "ui_dilby.h"
#include "downloader.h"
#include "settingsdialog.h"

Dilby::Dilby(QWidget *parent) : QMainWindow(parent), ui(new Ui::Dilby), settings("Huulivoide", "Dilby")
{
  // Set the icon theme if needed, before the gui is constructed,
  // so that icons can be loaded on Windows and OSX too
  if (QIcon::themeName().isEmpty())
    QIcon::setThemeName("TangoDilby");

  ui->setupUi(this);
  initSettings();

  baseDir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0) + "/";

  QDir bd;
  if (!bd.mkpath(baseDir))
  {
    QMessageBox::critical(this, tr("Error"), tr("Cannot create the data dir: %1").arg(baseDir));
    qApp->quit();
  }

  ui->comicView->setScene(&comic);
  comic.addItem(&pixmap);

  QMovie *loader = new QMovie(":/icons/loader.gif", QByteArray(), ui->loaderLabel);
  ui->loaderLabel->hide();
  ui->loaderLabel->setMovie(loader);

  ui->comicDate->setMaximumDate(QDate::currentDate());
  ui->comicDate->setDate(settings.value("currentDate").toDate());

  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(save()));
}

Dilby::~Dilby()
{
  delete ui;
}

QString Dilby::getComic(const QDate &date)
{
  QString dString(date.toString("yyyy-MM-dd"));
  QString fileName = baseDir + dString + prefix() + ".gif";

  if (QFile::exists(fileName))
    return fileName;

  Downloader dl;
  dl.setFile(QDir::tempPath() + "/Dilbert-" + dString + ".html");
  dl.setUrl("http://dilbert.com/strips/comic/" + dString);
  dl.download();

  QFile html(dl.fileName());
  if (!html.open(QIODevice::ReadOnly))
  {
    QMessageBox::critical(this, tr("Error"), tr("Cannot open the html file for scraping."));
    return "";
  }

  QString data(html.readAll());
  html.remove();

  QRegExp re(regexString());
  re.indexIn(data);
  QString imgUrl = re.cap(0);

  if (imgUrl.isEmpty())
  {
    QMessageBox::critical(this, tr("Error"), tr("No image url could be scrapped."));
    return "";
  }

  dl.setUrl("http://dilbert.com" + imgUrl);
  dl.setFile(fileName);
  dl.download();

  QPixmap test;
  if (!test.load(fileName))
  {
    QMessageBox::critical(this, tr("Error"), tr("Error loading the image file: %1\nPlease try again.").arg(fileName));
    QFile(fileName).remove(); // Make sure we don't leave invaöid file in the cache
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
  QDate current = ui->comicDate->date(); // Check if we would go beond the 1st dilbert
  if (current.addDays(-1) >= ui->comicDate->minimumDate())
  {
    QDate d = ui->comicDate->date();
    ui->comicDate->setDate(d.addDays(-1));
  }
}

void Dilby::on_buttonNext_clicked()
{
  QDate current = ui->comicDate->date(); // We cannot go into future
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
                     .arg("<a href=\"https://github.com/Huulivoide/Dilby\">Github - Dilby</a>")
                     .arg("<a href=\"mailto:jesse.jaara@gmail.com\">jesse.jaara@gmail.com</a>"));
}

void Dilby::on_actionAbout_QT_triggered()
{
  QMessageBox::aboutQt(this);
}

void Dilby::on_action_Settings_triggered()
{
  SettingsDialog sDialog(settings, this);
  sDialog.exec();
}

void Dilby::initSettings()
{
  if (!settings.contains("currentDate"))
    settings.setValue("currentDate", QDate::currentDate());

  if (!settings.contains("useHG"))
    settings.setValue("useHG", true);
}

bool Dilby::useHG()
{
  return settings.value("useHG").toBool();
}

QString Dilby::regexString()
{
  if (useHG())
    return "(\\/[^\\s]*\\.zoom\\.gif)";

  return "(\\/[^\\s]*\\.print\\.gif)";
}

QString Dilby::prefix()
{
  if (useHG())
    return ".HG";

  return ".LOW";
}
