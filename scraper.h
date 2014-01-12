#ifndef SCRAPER_H
#define SCRAPER_H

#include <QObject>
#include <QDate>

class Scraper : public QObject
{
    Q_OBJECT
  public:
    virtual QString getComic(const QDate &date, int quality) const = 0;
    virtual QDate next(const QDate &current) const     = 0;
    virtual QDate previous(const QDate &current) const = 0;

    virtual const QDate & minDate() const = 0;
    virtual const QDate & maxDate() const = 0;

  signals:
    void error(const QString errorMsg) const;

  protected:
    explicit Scraper(QObject *parent = 0) : QObject(parent) {};
};

#endif // SCRAPER_H
