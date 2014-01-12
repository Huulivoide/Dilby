#include <QSettings>
#include <QDir>
#include <QList>
#include <QtMath>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "dilby.h"

SettingsDialog::SettingsDialog(QSettings &s, QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog), settings(s)
{
  ui->setupUi(this);

  ui->checkUseHG->setChecked(useHG());

  connect(this, SIGNAL(cacheSizeChanged()), this, SLOT(setCacheSize()));

  ui->cacheSizeLabel->setText(tr("Calculating cache size."));
  emit cacheSizeChanged();
}

SettingsDialog::~SettingsDialog()
{
  delete ui;
}

bool SettingsDialog::useHG()
{
  return settings.value("useHG").toBool();
}

void SettingsDialog::useHG(bool HG)
{
  settings.setValue("useHG", HG);
}

void SettingsDialog::setCacheSize()
{
  QDir cache(dynamic_cast<Dilby *>(parent())->getCacheDir());
  QFileInfoList files = cache.entryInfoList();

  qreal size = 0;
  foreach (QFileInfo fi, files)
  {
    size += fi.size();
  }

  QString prefix;
  if (size / qPow(1024, 2) >= 1)
  {
    size /= qPow(1024, 2);
    prefix = tr("MB");
  }
  else
  {
    size /= 1024;
    prefix = tr("KB");
  }

  ui->cacheSizeLabel->setText(tr("Cache size: %1%2").arg(size, 0, 'f', 2).arg(prefix));
}

void SettingsDialog::on_buttonOK_clicked()
{
    useHG(ui->checkUseHG->isChecked());
    this->close();
}

void SettingsDialog::on_buttonCancel_clicked()
{
    this->close();
}

void SettingsDialog::on_clearCacheButton_clicked()
{
    QDir cache(dynamic_cast<Dilby *>(parent())->getCacheDir());

    cache.removeRecursively();
    cache.mkpath(cache.path());

    emit cacheSizeChanged();
}
