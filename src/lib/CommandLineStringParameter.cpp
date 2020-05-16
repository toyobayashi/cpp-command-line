#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"
#include "EnvironmentVariable.hpp"
#include <cstddef>

namespace commandline {

  CommandLineStringParameter::CommandLineStringParameter(const CommandLineStringDefinition& definition):
    CommandLineParameterWithArgument(definition),
    _value(""),
    defaultValue(definition.defaultValue) {
    // validateDefaultValue(true);
  }

  CommandLineParameterKind CommandLineStringParameter::kind() const {
    return CommandLineParameterKind::String;
  }

  void CommandLineStringParameter::_setValue() {
    if (this->environmentVariable != "") {
      auto env = commandline::env();
      std::string environmentValue;
      if (env.find(this->environmentVariable) != env.end()) {
        this->_value = env.at(this->environmentVariable);
        return;
      }
    }

    if (this->defaultValue != "") {
      this->_value = this->defaultValue;
      return;
    }

    this->_value = "";
  }
  void CommandLineStringParameter::_setValue(bool data) {
    reportInvalidData(data);
  }
  void CommandLineStringParameter::_setValue(int64_t data) {
    reportInvalidData(data);
  }
  void CommandLineStringParameter::_setValue(const std::string& data) {
    this->_value = data;
  }
  void CommandLineStringParameter::_setValue(const std::vector<std::string>& data) {
    reportInvalidData(data);
  }

  void CommandLineStringParameter::_getSupplementaryNotes(std::vector<std::string>& supplementaryNotes) const {
    CommandLineParameterWithArgument::_getSupplementaryNotes(supplementaryNotes);
    
    if (this->defaultValue.length() < 160) {
      supplementaryNotes.push_back("The default value is \"" + this->defaultValue + "\".");
    }
  }

  void CommandLineStringParameter::appendToArgList(std::vector<std::string>& argList) const {
    if (this->_value != "") {
      argList.push_back(this->longName);
      argList.push_back(this->_value);
    }
  }

  const std::string& CommandLineStringParameter::value() const {
    return this->_value;
  }
}
