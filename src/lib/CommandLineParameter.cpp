#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"
#include <sstream>
#include <cstddef>

namespace commandline {
  CommandLineParameter::CommandLineParameter(const BaseCommandLineDefinition& definition):
    _hasValue(false),
    // _parserKey(""),
    longName(definition.parameterLongName),
    shortName(definition.parameterShortName),
    description(definition.description),
    required(definition.required),
    environmentVariable(definition.environmentVariable) {
    
    std::regex _longNameRegExp("^-(-[a-z0-9]+)+$");
    std::regex _shortNameRegExp("^-[a-zA-Z]$");
    std::regex _environmentVariableRegExp("^[A-Z_][A-Z0-9_]*$");
    std::smatch sm;

    if (!std::regex_match(this->longName, sm, _longNameRegExp)) {
      throw CommandLineError(INVALID_NAME, "Invalid name: \"" + this->longName + "\". The parameter long name must be lower-case and use dash delimiters (e.g. \"--do-a-thing\")");
    }

    if (this->shortName != "") {
      if (!std::regex_match(this->shortName, sm, _shortNameRegExp)) {
        throw CommandLineError(INVALID_NAME, "Invalid name: \"" + this->shortName + "\". The parameter short name must be a dash followed by a single upper-case or lower-case letter (e.g. \"-a\")");
      }
    }

    if (this->environmentVariable != "") {
      if (this->required) {
        throw CommandLineError(INVALID_ENV, "An \"environmentVariable\" cannot be specified for \"" + this->longName + "\" because it is a required parameter");
      }

      if (!std::regex_match(this->environmentVariable, sm, _environmentVariableRegExp)) {
        throw CommandLineError(INVALID_NAME, "Invalid environment variable name: \"" + this->environmentVariable + "\". The name must consist only of upper-case letters, numbers, and underscores. It may not start with a number.");
      }
    }
  }

  void CommandLineParameter::_getSupplementaryNotes(std::vector<std::string>& supplementaryNotes) const {
    if (this->environmentVariable != "") {
      supplementaryNotes.push_back("This parameter may alternatively specified via the " + this->environmentVariable
      + " environment variable.");
    }
  }

  void CommandLineParameter::reportInvalidData(bool data) const {
    std::string dataString = data ? "true" : "false";
    throw CommandLineError(UNEXPECTED_DATA, "Unexpected data object for parameter \"" + this->longName + "\": " + dataString);
  }

  void CommandLineParameter::reportInvalidData(int64_t data) const {
    std::string dataString = std::to_string(data);
    throw CommandLineError(UNEXPECTED_DATA, "Unexpected data object for parameter \"" + this->longName + "\": " + dataString);
  }

  void CommandLineParameter::reportInvalidData(const std::string& data) const {
    throw CommandLineError(UNEXPECTED_DATA, "Unexpected data object for parameter \"" + this->longName + "\": " + data);
  }

  void CommandLineParameter::reportInvalidData(const std::vector<std::string>& data) const {
    std::string dataString = formatStringArray(data);
    throw CommandLineError(UNEXPECTED_DATA, "Unexpected data object for parameter \"" + this->longName + "\": " + dataString);
  }

  void CommandLineParameter::validateDefaultValue(bool hasDefaultValue) const {
    if (this->required && hasDefaultValue) {
      throw CommandLineError(UNEXPECTED_DEFAULT_VALUE, "A default value cannot be specified for \"" + this->longName + "\" because it is a \"required\" parameter");
    }
  }

  std::string CommandLineParameter::formatStringArray(const std::vector<std::string>& arr) const {
    size_t len = arr.size();
    std::ostringstream oss;
    if (len == 0) {
      oss << "[]";
      return oss.str();
    }
    oss << "[ ";
    for (size_t i = 0; i < len; i++) {
      oss << "'" << arr[i] << "'";
      if (i != len - 1) {
        oss << ", ";
      }
    }
    oss << " ]";
    return oss.str();
  }

  bool CommandLineParameter::hasValue() const {
    return _hasValue;
  }
  void CommandLineParameter::setHasValue() {
    this->_hasValue = true;
  }
}
