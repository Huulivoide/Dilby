#ifndef DILBY_H
#define DILBY_H

#include <QMainWindow>
#include <QDate>
#include <QSettings>

#include "scraper.h"

namespace Ui
{
  class Dilby;
}

class Dilby : public QMainWindow
{
    Q_OBJECT

  public:
    explicit Dilby(QWidget *parent = 0);
    ~Dilby();

    const QString & getCacheDir() const { return cacheDir; };

  private slots:
    void setComic(const QDate &date);
    void save();
    void showError(QString msg);

    void on_comicDate_dateChanged(const QDate &date);

    void on_buttonPrevious_clicked();
    void on_buttonNext_clicked();

    void on_action_Quit_triggered();
    void on_action_About_triggered();
    void on_actionAbout_QT_triggered();
    void on_action_Settings_triggered();


  private:
    Ui::Dilby *ui;
    QSettings settings;
    QString cacheDir; // $HOME/.cache/Dilby. For caching the comics.
    QDate currentDate; // Currently displayd comic, so we can rollback to it if loading fails
    Scraper *scraper;

    void initSettings(); // Sets currentdate and HG=true if unset in settings file
    bool useHG();
};

#endif // DILBY_H
