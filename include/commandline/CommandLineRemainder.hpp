#ifndef __COMMAND_LINE_REMAINDER_HPP__
#define __COMMAND_LINE_REMAINDER_HPP__

#include "CommandLineDefinition.hpp"

namespace commandline {

class CommandLineRemainder {
 private:
  std::vector<std::string> _values;

 public:
  std::string argumentName;
  std::string description;

  CommandLineRemainder();
  CommandLineRemainder(const CommandLineRemainderDefinition& definition);

  std::vector<std::string> values() const;

  void _setValue(const std::vector<std::string>& data);

  void appendToArgList(std::vector<std::string>& argList);
};

}

#endif
