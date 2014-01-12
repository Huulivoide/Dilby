#include <QDir>
#include <QStandardPaths>
#include <QShortcut>
#include <QMessageBox>

#include <QMovie>
#include <QIcon>

#include "dilby.h"
#include "ui_dilby.h"
#include "settingsdialog.h"
#include "dilbert.h"

Dilby::Dilby(QWidget *parent) : QMainWindow(parent), ui(new Ui::Dilby), settings("Huulivoide", "Dilby")
{
  // Set the icon theme if needed, before the gui is constructed,
  // so that icons can be loaded on Windows and OSX too
  if (QIcon::themeName().isEmpty())
    QIcon::setThemeName("TangoDilby");

  ui->setupUi(this);
  initSettings();

  cacheDir = QStandardPaths::standardLocations(QStandardPaths::CacheLocation).at(0) + "/";

  scraper = new Dilbert(cacheDir, this);

  QDir bd;
  if (!bd.mkpath(cacheDir))
  {
    QMessageBox::critical(this, tr("Error"), tr("Cannot create the data dir: %1").arg(cacheDir));
    qApp->quit();
  }

  QMovie *loader = new QMovie(":/icons/loader.gif", QByteArray(), ui->loaderLabel);
  ui->loaderLabel->hide();
  ui->loaderLabel->setMovie(loader);

  ui->comicDate->setMaximumDate(scraper->maxDate());
  ui->comicDate->setDate(settings.value("currentDate").toDate());

  QShortcut *previousCtrl = new QShortcut(QKeySequence("Ctrl+P"), this);
  QShortcut *previous = new QShortcut(QKeySequence("J"), this);
  QShortcut *nextCtrl = new QShortcut(QKeySequence("Ctrl+N"), this);
  QShortcut *next = new QShortcut(QKeySequence("K"), this);

  connect(previousCtrl, SIGNAL(activated()), this, SLOT(on_buttonPrevious_clicked()));
  connect(previous, SIGNAL(activated()), this, SLOT(on_buttonPrevious_clicked()));
  connect(nextCtrl, SIGNAL(activated()), this, SLOT(on_buttonNext_clicked()));
  connect(next, SIGNAL(activated()), this, SLOT(on_buttonNext_clicked()));

  connect(scraper, SIGNAL(error(QString)), this, SLOT(showError(QString)));
  connect(qApp, SIGNAL(aboutToQuit()), this, SLOT(save()));
}

Dilby::~Dilby()
{
  delete ui;
}

void Dilby::setComic(const QDate &date)
{
  if (date != currentDate)
  {
    ui->comicView->hide();
    ui->loaderLabel->show();
    ui->loaderLabel->movie()->start();

    QString fileName = scraper->getComic(date, useHG());

    ui->loaderLabel->movie()->stop();
    ui->loaderLabel->hide();
    ui->comicView->show();


    if (!fileName.isEmpty())
    {
      currentDate = date;

      QPixmap imgData(fileName);
      ui->comicView->setPixmap(imgData);

      ui->comicView->setFixedHeight(imgData.height());
      ui->comicView->setFixedWidth(imgData.width());
      ui->centralWidget->updateGeometry();
      QSize size = this->sizeHint();
      this->setFixedSize(size);
    }
    else // It triggers a new signal to rerun this function but.......
      ui->comicDate->setDate(currentDate);
  }
}

void Dilby::save()
{
  settings.setValue("currentDate", currentDate);
}

void Dilby::showError(QString msg)
{
  QMessageBox::critical(this, tr("Error"), msg);
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
  ui->comicDate->setDate(scraper->previous(currentDate));
}

void Dilby::on_buttonNext_clicked()
{
  ui->comicDate->setDate(scraper->next(currentDate));
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
