#include "Nexpp/CommandLine/CommandLine.h"
#include <qlogging.h>
#include <stdexcept>

CommandLine::CommandLine(const QApplication &application)
{
  m_parser.setApplicationDescription(
      "Nexpp - A tool to simplify the generation of modern C++ project "
      "structures."
  );
  m_parser.addHelpOption();
  m_parser.addVersionOption();

  setup_options();
  m_parser.process(application);

  consume_options();
}

void CommandLine::setup_options()
{
  add_mode_option();
  add_project_name_option();
  add_destination_option();
  add_libraries_option();
  add_standards_option();
  add_flags_option();
}

void CommandLine::add_mode_option()
{
  QCommandLineOption mode_option(
      QStringList() << "m" << "mode",
      QApplication::translate(
          "main", "Defines how Nexpp will run: graphical interface (gui) or "
                  "command line interface (cli)."
      ),
      QApplication::translate("main", "cli/gui")
  );
  m_parser.addOption(mode_option);
}

void CommandLine::add_project_name_option()
{
  QCommandLineOption project_name_option(
      QStringList() << "n" << "name",
      QApplication::translate("main", "The name of your project"),
      QApplication::translate("main", "Project name")
  );
  m_parser.addOption(project_name_option);
}

void CommandLine::add_destination_option()
{
  QCommandLineOption destination_option(
      QStringList() << "d" << "destination",
      QApplication::translate(
          "main", "Specifies the output directory where the generated project "
                  "will be created."
      ),
      QApplication::translate("main", "path")
  );
  m_parser.addOption(destination_option);
}

void CommandLine::add_libraries_option()
{
  const QStringList allowed_libraries = {"qt", "gtest"};
  m_parser.addOption(create_option_with_allowed_values(
      QStringList() << "l" << "libraries",
      "Specifies the libraries to include in the generated project. Multiple "
      "values can be provided, separated by commas.",
      "libraries", allowed_libraries
  ));
}

void CommandLine::add_standards_option()
{
  const QStringList allowed_standards = {"14", "17", "20", "23"};
  m_parser.addOption(create_option_with_allowed_values(
      QStringList() << "s" << "standard",
      "Selects the C++ standard to use for the project. Defaults to C++20 if "
      "unspecified.",
      "standard", allowed_standards
  ));
}

void CommandLine::add_flags_option()
{
  QCommandLineOption flags_option(
      QStringList() << "f" << "flags",
      QApplication::translate(
          "main", "Enables additional strict compile-time flags (e.g., "
                  "warnings as errors, extra warnings)."
      )
  );
  m_parser.addOption(flags_option);
}

QCommandLineOption CommandLine::create_option_with_allowed_values(
    const QStringList &names, const QString &description,
    const QString &value_name, const QStringList &allowed_values
)
{
  const QString full_description = QString("%1\nAllowed values: %2")
                                       .arg(description)
                                       .arg(allowed_values.join(", "));

  return QCommandLineOption(names, full_description, value_name);
}

AppMode CommandLine::get_mode() const
{
  return m_mode;
}

QString CommandLine::get_project_name() const
{
  return m_project_name;
}

QString CommandLine::get_destination() const
{
  return m_destination;
}

QStringList CommandLine::get_libraries() const
{
  return m_libraries;
}

Standard CommandLine::get_standard() const
{
  return m_standard;
}

bool CommandLine::has_flags() const
{
  return m_has_flags;
}

void CommandLine::consume_options()
{
  QString mode_value = m_parser.value("m");
  if(mode_value.isEmpty()) {
    m_mode = AppMode::CLI;
  } else {
    if(mode_value.toLower() != "gui" && mode_value.toLower() != "cli") {
      throw std::runtime_error("Mode argument is invalid");
    }

    m_mode = (mode_value.toLower() == "gui") ? AppMode::GUI : AppMode::CLI;
  }

  if(m_parser.value("n").isEmpty()) {
    throw std::runtime_error("Project name is required (-n) !");
  }

  m_project_name = m_parser.value("n");

  if(m_parser.value("d").isEmpty()) {
    qWarning(
    ) << "Destination value not provided, creating on current directory...";
  }

  m_destination = m_parser.value("d").isEmpty() ? "./" : m_parser.value("d");

  QString libraries = m_parser.value("l");

  if(!libraries.isEmpty()) {
    QStringList libs = libraries.split(",");
    QStringList unique_libs;

    for(const auto &lib : libs) {
      QString lower_lib = lib.toLower();
      if(lower_lib != "qt" && lower_lib != "gtest") {
        throw std::runtime_error("Unrecognized library: " + lib.toStdString());
      }
      bool already_present = std::any_of(
          unique_libs.begin(), unique_libs.end(),
          [&](const QString &existing) {
            return existing.compare(lower_lib, Qt::CaseInsensitive) == 0;
          }
      );
      if(!already_present) {
        unique_libs.append(lower_lib);
      }
    }

    m_libraries = unique_libs;
  } else {
    m_libraries = QStringList();
  }

  QString standard = m_parser.value("s");

  int     standard_value = standard.toInt();

  if(standard_value != 14 && standard_value != 17 && standard_value != 20 &&
     standard_value != 23) {
    qWarning() << "Unrecognized standard, defaulting to 23...";
  }

  m_standard =
      standard.isEmpty() ? Standard::CPP23 : from_int(standard.toInt());

  m_has_flags = m_parser.isSet("f");
}
