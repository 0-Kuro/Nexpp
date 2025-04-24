#include "Nexpp/CommandLine/CommandLine.h"
#include "Nexpp/Data/CMakeBase.h"

#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char **argv)
{
  QApplication app(argc, argv);
  QApplication::setApplicationName("Nexpp");
  QApplication::setApplicationVersion("0.0.1");

  CommandLine command_line(app);

  CMakeBase   cmake_base;

  auto        result = cmake_base.setup_config(
      command_line.get_project_name().toStdString(),
      command_line.get_standard(), command_line.has_flags()
  );

  qDebug() << result;

  if(command_line.get_mode() == AppMode::CLI) {
    app.closeAllWindows();

    return 0;
  }

  return app.exec();
}
