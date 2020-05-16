#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"
#include "EnvironmentVariable.hpp"
#include <cstddef>

namespace commandline {

  CommandLineFlagParameter::CommandLineFlagParameter(const CommandLineFlagDefinition& definition):
    CommandLineParameter(definition), _value(false), defaultValue(definition.defaultValue) {
    // validateDefaultValue(true);
  }

  CommandLineParameterKind CommandLineFlagParameter::kind() const {
    return CommandLineParameterKind::Flag;
  }

  void CommandLineFlagParameter::_setValue() {
    if (this->environmentVariable != "") {
      auto env = commandline::env();
      std::string environmentValue;
      if (env.find(this->environmentVariable) != env.end() && (environmentValue = env.at(this->environmentVariable)) != "") {
        if (environmentValue != "0" && environmentValue != "1") {
          throw CommandLineError(INVALID_ENV_VALUE, "Invalid value \"" + environmentValue + "\" for the environment variable " + this->environmentVariable + ". Valid choices are: 0 or 1");
        }
        this->_value = environmentValue == "1";
        return;
      }
    }

    this->_value = this->defaultValue;
  }

  void CommandLineFlagParameter::_setValue(bool data) {
    this->_value = data;
  }
  void CommandLineFlagParameter::_setValue(int64_t data) {
    reportInvalidData(data);
  }
  void CommandLineFlagParameter::_setValue(const std::string& data) {
    if (data == "true" || data == "1") {
      this->_value = true;
    } else if (data == "false" || data == "0") {
      this->_value = false;
    } else {
      reportInvalidData(data);
    }
  }
  void CommandLineFlagParameter::_setValue(const std::vector<std::string>& data) {
    reportInvalidData(data);
  }

  void CommandLineFlagParameter::_getSupplementaryNotes(std::vector<std::string>& supplementaryNotes) const {
    CommandLineParameter::_getSupplementaryNotes(supplementaryNotes);
    
    supplementaryNotes.push_back("The default value is \"" + this->defaultValue ? std::string("true") : std::string("false") + "\".");
  }

  void CommandLineFlagParameter::appendToArgList(std::vector<std::string>& argList) const {
    if (this->_value) {
      argList.push_back(this->longName);
    }
  }

  bool CommandLineFlagParameter::value() const {
    return this->_value;
  }
}
