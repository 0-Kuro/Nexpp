#pragma once

#include <string>

#include "Nexpp/Types/Standard.h"

class CMakeBase
{
public:
  std::string setup_config(
      const std::string &project_name, Standard cpp_standard, bool has_flags
  ) const;
};
