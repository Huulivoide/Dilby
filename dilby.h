#ifndef DILBY_H
#define DILBY_H

#include <QMainWindow>
#include <QDate>
#include <QSettings>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

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

  private slots:
    void on_action_Quit_triggered();
    void on_comicDate_dateChanged(const QDate &date);

    void on_buttonPrevious_clicked();
    void on_buttonNext_clicked();

    void setComic(const QDate &date);

    void save();

    void on_action_About_triggered();

    void on_actionAbout_QT_triggered();

  private:
    Ui::Dilby *ui;
    QSettings settings;
    QString baseDir; // $HOME/.cache/Dilby. For caching the comics.
    QDate currentDate; // Currently displayd comic, so we can rollback to it if loading fails

    QGraphicsScene comic;
    QGraphicsPixmapItem pixmap;

    QString getComic(const QDate &date);
};

#endif // DILBY_H
