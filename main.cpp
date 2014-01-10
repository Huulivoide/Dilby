#include "dilby.h"
#include <QApplication>
#include <QTranslator>
#include <QLibraryInfo>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QTranslator qtTranslator;
  qtTranslator.load("qt_" + QLocale::system().name(),
          QLibraryInfo::location(QLibraryInfo::TranslationsPath));
  a.installTranslator(&qtTranslator);

  QTranslator myappTranslator;
  myappTranslator.load(":/i18n/" + QLocale::system().name());
  a.installTranslator(&myappTranslator);


  Dilby w;
  w.show();

  return a.exec();
}
