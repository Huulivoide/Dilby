#include <QSettings>

#include "settingsdialog.h"
#include "ui_settingsdialog.h"

SettingsDialog::SettingsDialog(QSettings &s, QWidget *parent) :
  QDialog(parent), ui(new Ui::SettingsDialog), settings(s)
{
  ui->setupUi(this);

  ui->checkUseHG->setChecked(useHG());
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

void SettingsDialog::on_buttonOK_clicked()
{
    useHG(ui->checkUseHG->isChecked());
    this->close();
}

void SettingsDialog::on_buttonCancel_clicked()
{
    this->close();
}
