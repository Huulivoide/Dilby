#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QSettings>

namespace Ui
{
  class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit SettingsDialog(QSettings &s, QWidget *parent = 0);
    ~SettingsDialog();

  signals:
    void cacheSizeChanged();

  private slots:
    void setCacheSize();

    void on_buttonOK_clicked();
    void on_buttonCancel_clicked();

    void on_clearCacheButton_clicked();

  private:
    Ui::SettingsDialog *ui;
    QSettings &settings;

    bool useHG(); // Return true if using high quality comics, fale for low res ones
    void useHG(bool HG);
};

#endif // SETTINGSDIALOG_H
