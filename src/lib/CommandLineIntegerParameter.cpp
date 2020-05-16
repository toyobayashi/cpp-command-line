#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"
#include "EnvironmentVariable.hpp"
#include <cstddef>

namespace commandline {

  CommandLineIntegerParameter::CommandLineIntegerParameter(const CommandLineIntegerDefinition& definition):
    CommandLineParameterWithArgument(definition),
    _value(0),
    defaultValue(definition.defaultValue) {

    // validateDefaultValue(true);
  }

  CommandLineParameterKind CommandLineIntegerParameter::kind() const {
    return CommandLineParameterKind::Integer;
  }

  void CommandLineIntegerParameter::_setValue() {
    if (this->environmentVariable != "") {
      auto env = commandline::env();
      std::string environmentValue;
      if (env.find(this->environmentVariable) != env.end() && (environmentValue = env.at(this->environmentVariable)) != "") {
        int64_t parsed;
        if (environmentValue == "0") {
          parsed = 0;
        } else {
          parsed = std::strtoll(environmentValue.c_str(), nullptr, 10);
          if (parsed == 0) {
            throw CommandLineError(INVALID_ENV_VALUE, "Invalid value \"" + environmentValue + "\" for the environment variable " + this->environmentVariable + ". It must be an integer value.");
          }
        }
        this->_value = parsed;
        return;
      }
    }

    if (this->defaultValue != 0) {
      this->_value = this->defaultValue;
      return;
    }

    this->_value = 0;
  }
  void CommandLineIntegerParameter::_setValue(bool data) {
    reportInvalidData(data);
  }
  void CommandLineIntegerParameter::_setValue(int64_t data) {
    this->_value = data;
  }
  void CommandLineIntegerParameter::_setValue(const std::string& data) {
    if (data == "0") {
      this->_value = 0;
      return;
    }
    
    int64_t v = std::strtoll(data.c_str(), nullptr, 10);
    if (v == 0) {
      reportInvalidData(data);
    } else {
      this->_value = v;
    }
  }
  void CommandLineIntegerParameter::_setValue(const std::vector<std::string>& data) {
    reportInvalidData(data);
  }

  void CommandLineIntegerParameter::_getSupplementaryNotes(std::vector<std::string>& supplementaryNotes) const {
    CommandLineParameterWithArgument::_getSupplementaryNotes(supplementaryNotes);
    
    supplementaryNotes.push_back("The default value is \"" + std::to_string(this->defaultValue) + "\".");
  }

  void CommandLineIntegerParameter::appendToArgList(std::vector<std::string>& argList) const {
    argList.push_back(this->longName);
    argList.push_back(std::to_string(this->_value));
  }

  int64_t CommandLineIntegerParameter::value() const {
    return this->_value;
  }
}
