#pragma once

#include <QApplication>
#include <QCommandLineParser>

#include "Nexpp/Types/AppMode.h"
#include "Nexpp/Types/Standard.h"

class CommandLine
{
public:
  explicit CommandLine(const QApplication &application);

  AppMode     get_mode() const;
  QString     get_project_name() const;
  QString     get_destination() const;
  QStringList get_libraries() const;
  Standard    get_standard() const;
  bool        has_flags() const;

private:
  void               setup_options();

  void               add_mode_option();
  void               add_project_name_option();
  void               add_destination_option();
  void               add_libraries_option();
  void               add_standards_option();
  void               add_flags_option();

  QCommandLineOption create_option_with_allowed_values(
      const QStringList &names, const QString &description,
      const QString &value_name, const QStringList &allowed_values
  );

  void               consume_options();

  QCommandLineParser m_parser;

  AppMode            m_mode;
  QString            m_project_name;
  QString            m_destination;
  QStringList        m_libraries;
  Standard           m_standard;
  bool               m_has_flags;
};
