#ifndef DILBERT_H
#define DILBERT_H

#include <QObject>

#include "scraper.h"

class Dilbert : public Scraper
{
    Q_OBJECT
  public:
    explicit Dilbert(const QString &cd, QObject *parent = 0);

    QString getComic(const QDate &date, int quality) const;
    QDate next(const QDate &current) const;
    QDate previous(const QDate &current) const;

    const QDate & minDate() const { return min; };
    const QDate & maxDate() const { return max; };

  private:
    static QDate min;
    static QDate max;

    QString cacheDir;

    QString prefix(int quality) const;
    QString regexString(int quality) const;
};

#endif // DILBERT_H
