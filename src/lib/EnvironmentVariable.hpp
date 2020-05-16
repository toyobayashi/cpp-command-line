#ifndef __ENVIRONMENT_VARIABLE_HPP__
#define __ENVIRONMENT_VARIABLE_HPP__

#include <string>
#include <map>

namespace commandline {
  const std::map<std::string, std::string>& env();
}

#endif
