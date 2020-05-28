#include "commandline/CommandLineRemainder.hpp"

namespace commandline {

CommandLineRemainder::CommandLineRemainder(const CommandLineRemainderDefinition& definition): CommandLineRemainder() {
  this->argumentName = definition.argumentName;
  this->description = definition.description;
}

CommandLineRemainder::CommandLineRemainder(): _values(), argumentName("..."), description("") {}

std::vector<std::string> CommandLineRemainder::values() const {
  return this->_values;
}

void CommandLineRemainder::_setValue(const std::vector<std::string>& data) {
    for (const auto& v : data) {
      this->_values.push_back(v);
    }
  }

void CommandLineRemainder::appendToArgList(std::vector<std::string>& argList) {
  if (this->_values.size() > 0) {
    for (const auto& value : this->_values) {
      argList.push_back(value);
    }
  }
}

}
