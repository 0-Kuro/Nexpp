#include "Nexpp/CommandLine/CommandLine.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("Nexpp");
  QApplication::setApplicationVersion("0.0.1");

  CommandLine command_line(app);

  return app.exec();
}
