#pragma once

#include <QString>

enum class AppMode
{
  GUI,
  CLI
};

inline const QString to_string(AppMode mode) noexcept
{
  switch(mode) {
  case AppMode::GUI:
    return "GUI";
  case AppMode::CLI:
    return "CLI";
  default:
    return "Invalid";
  }
}
