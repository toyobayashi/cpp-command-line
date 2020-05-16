#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"
#include "EnvironmentVariable.hpp"
#include <cstddef>

namespace commandline {

  CommandLineStringListParameter::CommandLineStringListParameter(const CommandLineStringListDefinition& definition):
    CommandLineParameterWithArgument(definition),
    _values() {}

  CommandLineParameterKind CommandLineStringListParameter::kind() const {
    return CommandLineParameterKind::StringList;
  }

  void CommandLineStringListParameter::_setValue() {
    if (this->environmentVariable != "") {
      auto env = commandline::env();
      std::string environmentValue;
      if (env.find(this->environmentVariable) != env.end()) {
        this->_values = { env.at(this->environmentVariable) };
        return;
      }
    }

    this->_values = {};
  }
  void CommandLineStringListParameter::_setValue(bool data) {
    reportInvalidData(data);
  }
  void CommandLineStringListParameter::_setValue(int64_t data) {
    reportInvalidData(data);
  }
  void CommandLineStringListParameter::_setValue(const std::string& data) {
    // reportInvalidData(data);
    this->_values.push_back(data);
  }
  void CommandLineStringListParameter::_setValue(const std::vector<std::string>& data) {
    this->_values = data;
  }

  void CommandLineStringListParameter::appendToArgList(std::vector<std::string>& argList) const {
    if (this->_values.size() > 0) {
      for (const auto& value : this->_values) {
        argList.push_back(this->longName);
        argList.push_back(value);
      }
    }
  }

  const std::vector<std::string>& CommandLineStringListParameter::values() const {
    return this->_values;
  }
}
