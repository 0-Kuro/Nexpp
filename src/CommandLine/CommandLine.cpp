#include "Nexpp/CommandLine/CommandLine.h"

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

// todo(Darleanow): New MR for arg checks
void CommandLine::consume_options()
{
  QString mode_value = m_parser.value("m");
  if(mode_value.isEmpty()) {
    m_mode = AppMode::CLI;
  } else {
    m_mode = (mode_value.toLower() == "gui") ? AppMode::GUI : AppMode::CLI;
  }

  m_project_name =
      m_parser.value("n").isEmpty() ? "NexppProject" : m_parser.value("n");

  m_destination = m_parser.value("d").isEmpty() ? "./" : m_parser.value("d");

  QString libraries = m_parser.value("l");
  m_libraries = libraries.isEmpty() ? QStringList() : libraries.split(",");

  QString standard = m_parser.value("s");
  m_standard =
      standard.isEmpty() ? Standard::CPP23 : from_int(standard.toInt());

  m_has_flags = m_parser.isSet("f");
}
