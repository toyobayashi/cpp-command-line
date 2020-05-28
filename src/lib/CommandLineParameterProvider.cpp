#include "commandline/CommandLineParameterProvider.hpp"
#include "commandline/CommandLineError.hpp"
#include "StringUtil.hpp"

namespace commandline {

CommandLineParameterProvider::CommandLineParameterProvider():
  _parameters(),
  _parametersByLongName(),
  _parametersByShortName(),
  _remainder(nullptr) {}

CommandLineParameterProvider::~CommandLineParameterProvider() {
  if (_remainder != nullptr) {
    delete _remainder;
  }
  for (CommandLineParameter* p : this->_parameters) {
    delete p;
  }
}

const std::vector<CommandLineParameter*>& CommandLineParameterProvider::parameters() const {
  return this->_parameters;
}

const CommandLineRemainder* CommandLineParameterProvider::remainder() const {
  return this->_remainder;
}

const CommandLineRemainder* CommandLineParameterProvider::defineCommandLineRemainder(const CommandLineRemainderDefinition& definition) {
  if (this->_remainder != nullptr) {
    throw CommandLineError(REMAINDER_DEFINED, "defineRemainingArguments() has already been called for this provider");
  }
  this->_remainder = new CommandLineRemainder(definition);

  return this->_remainder;
}

CommandLineChoiceParameter* CommandLineParameterProvider::defineChoiceParameter(const CommandLineChoiceDefinition& definition) {
  CommandLineChoiceParameter* parameter = new CommandLineChoiceParameter(definition);
  this->_defineParameter(parameter);
  return parameter;
}

const CommandLineChoiceParameter* CommandLineParameterProvider::getChoiceParameter(const std::string& parameterName) const {
  return static_cast<const CommandLineChoiceParameter*>(this->_getParameter(parameterName, CommandLineParameterKind::Choice));
}

CommandLineFlagParameter* CommandLineParameterProvider::defineFlagParameter(const CommandLineFlagDefinition& definition) {
  CommandLineFlagParameter* parameter = new CommandLineFlagParameter(definition);
  this->_defineParameter(parameter);
  return parameter;
}

const CommandLineFlagParameter* CommandLineParameterProvider::getFlagParameter(const std::string& parameterName) const {
  return static_cast<const CommandLineFlagParameter*>(this->_getParameter(parameterName, CommandLineParameterKind::Flag));
}

CommandLineIntegerParameter* CommandLineParameterProvider::defineIntegerParameter(const CommandLineIntegerDefinition& definition) {
  CommandLineIntegerParameter* parameter = new CommandLineIntegerParameter(definition);
  this->_defineParameter(parameter);
  return parameter;
}

const CommandLineIntegerParameter* CommandLineParameterProvider::getIntegerParameter(const std::string& parameterName) const {
  return static_cast<const CommandLineIntegerParameter*>(this->_getParameter(parameterName, CommandLineParameterKind::Integer));
}

CommandLineStringParameter* CommandLineParameterProvider::defineStringParameter(const CommandLineStringDefinition& definition) {
  CommandLineStringParameter* parameter = new CommandLineStringParameter(definition);
  this->_defineParameter(parameter);
  return parameter;
}
const CommandLineStringParameter* CommandLineParameterProvider::getStringParameter(const std::string& parameterName) const {
  return static_cast<const CommandLineStringParameter*>(this->_getParameter(parameterName, CommandLineParameterKind::String));
}

CommandLineStringListParameter* CommandLineParameterProvider::defineStringListParameter(const CommandLineStringListDefinition& definition) {
  CommandLineStringListParameter* parameter = new CommandLineStringListParameter(definition);
  this->_defineParameter(parameter);
  return parameter;
}
const CommandLineStringListParameter* CommandLineParameterProvider::getStringListParameter(const std::string& parameterName) const {
  return static_cast<const CommandLineStringListParameter*>(this->_getParameter(parameterName, CommandLineParameterKind::StringList));
}

CommandLineParameter* CommandLineParameterProvider::_tryGetParameter(const std::string& parameterName) const {
  if (this->_parametersByLongName.find(parameterName) == this->_parametersByLongName.end()) {
    if (this->_parametersByShortName.find(parameterName) == this->_parametersByShortName.end()) {
      return nullptr;
    } else {
      return this->_parametersByShortName.at(parameterName);
    }
  } else {
    return this->_parametersByLongName.at(parameterName);
  }
}

CommandLineParameter* CommandLineParameterProvider::_getParameter(const std::string& parameterName) const {
  CommandLineParameter* parameter = this->_tryGetParameter(parameterName);
  if (parameter == nullptr) {
    throw CommandLineError(PARAMETER_UNDEFINED, "The parameter \"" + parameterName + "\" is not defined");
  }
  return parameter;
}

const CommandLineParameter* CommandLineParameterProvider::_getParameter(const std::string& parameterName, CommandLineParameterKind expectedKind) const {
  const CommandLineParameter* parameter = this->_getParameter(parameterName);
  CommandLineParameterKind kind = parameter->kind();
  if (kind != expectedKind) {
    throw CommandLineError(PARAMETER_TYPE_ERROR, "The parameter \"" + parameterName+ "\" is of type \"" + _kindToString(kind) + "\" whereas the caller was expecting \"" + _kindToString(expectedKind) + "\".");
  }
  return parameter;
}

std::string CommandLineParameterProvider::_kindToString(CommandLineParameterKind kind) {
  std::vector<std::string> list = {
    "Choice",
    "Flag",
    "Integer",
    "String",
    "StringList"
  };
  return list[kind];
}

std::string CommandLineParameterProvider::_kindToString(const CommandLineParameter* p) {
  CommandLineParameterKind kind = p->kind();
  if (kind == CommandLineParameterKind::Choice) {
    const CommandLineChoiceParameter* choiceParameter = static_cast<const CommandLineChoiceParameter*>(p);
    return "Choice " + choiceParameter->formatAlternatives();
  }
  return _kindToString(kind);
}

void CommandLineParameterProvider::_defineParameter(CommandLineParameter* parameter) {
  if (this->_remainder) {
    throw CommandLineError(REMAINDER_DEFINED, "defineCommandLineRemainder() was already called for this provider; no further parameters can be defined");
  }
  this->_parameters.push_back(parameter);
  this->_parametersByLongName[parameter->longName] = parameter;
  if (parameter->shortName != "") {
    this->_parametersByShortName[parameter->shortName] = parameter;
  }
}

std::string CommandLineParameterProvider::_defaultValueToString(const CommandLineParameter* parameter) {
  CommandLineParameterKind kind = parameter->kind();
  switch (kind) {
    case CommandLineParameterKind::Choice:
      return static_cast<const CommandLineChoiceParameter*>(parameter)->defaultValue;
    case CommandLineParameterKind::String:
      return static_cast<const CommandLineStringParameter*>(parameter)->defaultValue;
    case CommandLineParameterKind::Flag:
      return static_cast<const CommandLineFlagParameter*>(parameter)->defaultValue ? "true" : "false";
    case CommandLineParameterKind::Integer:
      return std::to_string(static_cast<const CommandLineIntegerParameter*>(parameter)->defaultValue);
    case CommandLineParameterKind::StringList:
      return "[]";
    default:
      throw CommandLineError(PARAMETER_TYPE_UNKNOWN, "Unknown parameter kind");
  }
}

void CommandLineParameterProvider::_processArgs(const std::vector<std::string>& args) {
  auto isInteger = [](const std::string& arg) -> bool {
    std::regex re("^-?[1-9]\\d*$");
    std::smatch sm;
    return std::regex_match(arg, sm, re);
  };
  for (CommandLineParameter* p : this->_parameters) {
    if (!p->required) {
      p->_setValue();
    }
  }
  size_t i = 0;
  for (; i < args.size(); i++) {
    const std::string& arg = args[i];
    if (arg.length() > 2 && arg[0] == '-' && arg[1] == '-') { // --name
      if (arg.find("=") != std::string::npos) {
        std::vector<std::string> arr = commandline::string::split(arg, "=", -1);
        const std::string& key = arr[0];
        const std::string& value = arr[1];
        CommandLineParameter* parameter = this->_getParameter(key);
        parameter->_setValue(value);
        parameter->setHasValue();
      } else {
        if (args.size() > i + 1 && ((args[i + 1].length() > 0 && args[i + 1][0] != '-') || isInteger(args[i + 1]))) {
          CommandLineParameter* parameter = this->_getParameter(arg);
          if (parameter->kind() == CommandLineParameterKind::Flag && this->_remainder != nullptr) {
            parameter->_setValue(true);
            parameter->setHasValue();
          } else {
            parameter->_setValue(args[i + 1]);
            parameter->setHasValue();
            i++;
          }
        } else {
          CommandLineParameter* parameter = this->_getParameter(arg);
          parameter->_setValue(true);
          parameter->setHasValue();
        }
      }
    } else if (arg[0] == '-' && arg.length() > 1 && arg[1] != '-') { // -nxxx
      if (arg.length() == 2) { // -n
        // -s [nextArg]
        if (args.size() > i + 1) {
          const std::string& nextArg = args[i + 1];
          if (nextArg != "") {
            if (nextArg[0] != '-') {
              CommandLineParameter* parameter = this->_getParameter(arg);
              parameter->_setValue(nextArg);
              parameter->setHasValue();
              i++;
            } else {
              if (nextArg.length() > 1 && isInteger(nextArg)) {
                CommandLineParameter* parameter = this->_getParameter(arg);
                parameter->_setValue((int64_t)std::strtoll(nextArg.c_str(), nullptr, 10));
                parameter->setHasValue();
                i++;
              } else {
                CommandLineParameter* parameter = this->_getParameter(arg);
                parameter->_setValue(true);
                parameter->setHasValue();
              }
            }
          } else {
            CommandLineParameter* parameter = this->_getParameter(arg);
            parameter->_setValue(true);
            parameter->setHasValue();
          }
        } else {
          CommandLineParameter* parameter = this->_getParameter(arg);
          parameter->_setValue(true);
          parameter->setHasValue();
        }
      } else {
        // -a9000
        const std::string shortname = arg.substr(0, 2);
        const std::string value = arg.substr(2);
        CommandLineParameter* parameter = this->_getParameter(shortname);
        parameter->_setValue(value);
        parameter->setHasValue();
      }
    } else {
      break;
    }
  }
  
  if (this->_remainder != nullptr) {
    std::vector<std::string> remain;
    for (; i < args.size(); i++) {
      remain.push_back(args[i]);
    }
    this->_remainder->_setValue(remain);
  }

  for (CommandLineParameter* p : this->_parameters) {
    if (p->required) {
      if (!p->hasValue()) {
        throw CommandLineError(VALUE_REQUIRED, std::string() + "Required: --" + p->longName + " " + (p->shortName != "" ? ("(-" + p->shortName + ") ") : " ") + "[" + _kindToString(p) + "]");
      }
    }
  }
}

}
