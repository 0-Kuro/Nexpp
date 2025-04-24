#pragma once

#include <QString>

enum class Standard
{
  CPP14,
  CPP17,
  CPP20,
  CPP23
};

inline const QString to_string(Standard standard) noexcept
{
  switch(standard) {
  case Standard::CPP14:
    return "14";
  case Standard::CPP17:
    return "17";
  case Standard::CPP20:
    return "20";
  case Standard::CPP23:
    return "23";
  default:
    return "Invalid";
  }
}

inline Standard from_int(int standard) noexcept
{
  if(standard == 14) {
    return Standard::CPP14;
  }

  if(standard == 17) {
    return Standard::CPP17;
  }

  if(standard == 20) {
    return Standard::CPP20;
  }

  return Standard::CPP23;
}
