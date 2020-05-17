#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"
#include "EnvironmentVariable.hpp"
#include "StringUtil.hpp"
#include <cstddef>

#include <sstream>

namespace commandline {

  CommandLineChoiceParameter::CommandLineChoiceParameter(const CommandLineChoiceDefinition& definition):
    CommandLineParameter(definition),
    _value(""),
    alternatives(definition.alternatives),
    defaultValue("") {
    if (definition.alternatives.size() < 1) {
      throw CommandLineError(EMPTY_ALTERNATIVE_LIST, "When defining a choice parameter, the alternatives list must contain at least one value.)");
    }

    if (definition.defaultValue != "") {
      if (commandline::string::indexOf(definition.alternatives, definition.defaultValue) == -1) {
        throw CommandLineError(ERROR_ALTERNATIVE_DEFAULT_VALUE, "The specified default value \"" + definition.defaultValue + "\" is not one of the available options: " + formatStringArray(definition.alternatives));
      } else {
        this->defaultValue = definition.defaultValue;
      }
    } else {
      this->defaultValue = this->alternatives[0];
    }

    // validateDefaultValue(this->defaultValue != "");
  }

  CommandLineParameterKind CommandLineChoiceParameter::kind() const {
    return CommandLineParameterKind::Choice;
  }

  void CommandLineChoiceParameter::_setValue() {
    if (this->environmentVariable != "") {
      auto env = commandline::env();
      std::string environmentValue;
      if (env.find(this->environmentVariable) != env.end() && (environmentValue = env.at(this->environmentVariable)) != "") {
        if (commandline::string::indexOf(this->alternatives, environmentValue) < 0) {
          throw CommandLineError(INVALID_ENV_VALUE, "Invalid value \"" + environmentValue + "\" for the environment variable " + this->environmentVariable + ". Valid choices are: " + formatStringArray(this->alternatives));
        }
        this->_value = environmentValue;
        return;
      }
    }

    if (this->defaultValue != "") {
      this->_value = this->defaultValue;
      return;
    }

    this->_value = this->alternatives[0];
  }
  void CommandLineChoiceParameter::_setValue(bool data) {
    reportInvalidData(data);
  }
  void CommandLineChoiceParameter::_setValue(int64_t data) {
    reportInvalidData(data);
  }
  void CommandLineChoiceParameter::_setValue(const std::string& data) {
    if (commandline::string::indexOf(this->alternatives, data) < 0) {
      throw CommandLineError(INVALID_VALUE, "Invalid value \"" + data + "\" for the parameter " + this->longName + ". Valid choices are: " + formatStringArray(this->alternatives));
    }
    this->_value = data;
  }
  void CommandLineChoiceParameter::_setValue(const std::vector<std::string>& data) {
    reportInvalidData(data);
  }

  void CommandLineChoiceParameter::_getSupplementaryNotes(std::vector<std::string>& supplementaryNotes) const {
    CommandLineParameter::_getSupplementaryNotes(supplementaryNotes);
    
    if (this->defaultValue != "") {
      supplementaryNotes.push_back("The default value is \"" + this->defaultValue + "\".");
    }
  }

  void CommandLineChoiceParameter::appendToArgList(std::vector<std::string>& argList) const {
    if (this->_value != "") {
      argList.push_back(this->longName);
      argList.push_back(this->_value);
    }
  }

  const std::string& CommandLineChoiceParameter::value() const {
    return this->_value;
  }

  std::string CommandLineChoiceParameter::formatAlternatives() const {
    size_t len = this->alternatives.size();
    std::ostringstream oss;
    if (len == 0) {
      oss << "{}";
      return oss.str();
    }
    oss << "{";
    for (size_t i = 0; i < len; i++) {
      oss << this->alternatives[i];
      if (i != len - 1) {
        oss << ",";
      }
    }
    oss << "}";
    return oss.str();
  }
}
