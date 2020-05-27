#include "commandline/CommandLineAction.hpp"
#include "commandline/CommandLineError.hpp"
#include <iostream>
namespace commandline {

std::regex CommandLineAction::_actionNameRegExp("^[a-z][a-z0-9]*([-:][a-z0-9]+)*$");

CommandLineAction::CommandLineAction():
  CommandLineParameterProvider(),
  actionName(),
  summary(),
  documentation() {}

CommandLineAction::CommandLineAction(const CommandLineActionOptions& options):
  CommandLineAction() {
  _init(options);
}

CommandLineAction::~CommandLineAction() {}

void CommandLineAction::_init(const CommandLineActionOptions& options) {
  actionName = options.actionName;
  summary = options.summary;
  documentation = options.documentation;

  std::smatch sm;
  if (!std::regex_match(options.actionName, sm, _actionNameRegExp)) {
    throw CommandLineError(INVALID_NAME, "Invalid action name \"" + options.actionName + "\". The name must be comprised of lower-case words optionally separated by hyphens or colons.");
  }
}

void CommandLineAction::_buildParser() {
  this->onDefineParameters();
}
void CommandLineAction::_execute() {
  this->onExecute();
}

void CommandLineAction::_processArgs(const std::vector<std::string>& args) {
  CommandLineParameterProvider::_processArgs(args);
}

std::string CommandLineAction::renderHelpText(const std::string& toolFilename) const {
  auto repeat = [](const std::string& c, size_t times) {
    std::string res = "";
    for (size_t i = 0; i < times; i++) {
      res += c;
    }
    return res;
  };

#ifdef _WIN32
  std::string EOL = "\r\n";
#else
  const std::string EOL = "\n";
#endif
  std::string usage = "usage: " + toolFilename + " " + this->actionName + " ";
  size_t indent = usage.length();
  usage += "[-h]";

  const std::vector<CommandLineParameter*>& parameters = this->parameters();

  std::string shortOptions = "";
  for (size_t i = 0; i < parameters.size(); i++) {
    const CommandLineParameter* p = parameters[i];
    std::string optionString = " ";
    if (!p->required) {
      optionString += "[";
    }
    if (p->shortName != "") {
      optionString += p->shortName;
    } else {
      optionString += p->longName;
    }

    CommandLineParameterKind kind = p->kind();
    switch (kind) {
      case CommandLineParameterKind::Choice:
        optionString += " " + static_cast<const CommandLineChoiceParameter*>(p)->formatAlternatives();
        break;
      case CommandLineParameterKind::Flag:
        break;
      case CommandLineParameterKind::Integer: {
        const CommandLineIntegerParameter* param = static_cast<const CommandLineIntegerParameter*>(p);
        if (param->argumentName != "") {
          optionString += " " + param->argumentName;
        }
        break;
      }
      case CommandLineParameterKind::String: {
        const CommandLineStringParameter* param = static_cast<const CommandLineStringParameter*>(p);
        if (param->argumentName != "") {
          optionString += " " + param->argumentName;
        }
        break;
      }
      case CommandLineParameterKind::StringList: {
        const CommandLineStringListParameter* param = static_cast<const CommandLineStringListParameter*>(p);
        if (param->argumentName != "") {
          optionString += " " + param->argumentName;
        }
        break;
      }
      default:
        break;
    }

    if (!p->required) {
      optionString += "]";
    }

    shortOptions += optionString;
    if (i % 2 == 1) {
      shortOptions += EOL + repeat(" ", indent - 1);
    }
  }

  usage += shortOptions + EOL + EOL + this->documentation + EOL + EOL;

  std::string optionPart = "Options:" + EOL;
  const size_t len = indent;
  std::string front = "  -h, --help";
  std::string space = len < front.length() + 1 ? EOL + repeat(" ", len) : repeat(" ", len - front.length());
  optionPart += front + space + "Show this help message and exit." + EOL;
  if (parameters.size() > 0) {
    for (CommandLineParameter* p : parameters) {
      std::string front = repeat(" ", 2) + (p->shortName != "" ? p->shortName + ", " : "") + p->longName + (p->environmentVariable != "" ? ", $" + p->environmentVariable : "") + " [" + _kindToString(p) + "]" + (p->required ? " <REQUIRED>" : (std::string(" (") + _defaultValueToString(p) + ")"));
      std::string space = len < front.length() + 1 ? EOL + repeat(" ", len) : repeat(" ", len - front.length());
      optionPart += front + space + p->description + EOL;
    }
  }

  return usage + optionPart;
}

void* CommandLineAction::operator new(size_t size) {
  return ::operator new(size);
}
void* CommandLineAction::operator new[](size_t size) {
  return ::operator new(size);
}
void CommandLineAction::operator delete(void* p) noexcept {
  ::operator delete(p);
}
void CommandLineAction::operator delete[](void* p) noexcept {
  ::operator delete(p);
}

}
