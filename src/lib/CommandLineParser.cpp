#include "commandline/CommandLineParser.hpp"
#include "commandline/CommandLineError.hpp"
#include "StringUtil.hpp"
#include <cstddef>
#include <iostream>

namespace commandline {

std::string CommandLineParser::_getName() const {
  return this->toolFilename;
}

std::string CommandLineParser::_getDescription() const {
  return this->toolDescription;
}

CommandLineParser::CommandLineParser():
  CommandLineParameterProvider(),
  _options(),
  _actions(),
  _actionsByName(),
  _executed(false),
  toolFilename(""),
  toolDescription(""),
  selectedAction(nullptr) {

  // this->onDefineParameters();
}

CommandLineParser::CommandLineParser(const CommandLineParserOptions& options):
  CommandLineParser() {
  _init(options);
  // this->onDefineParameters();
}

CommandLineParser::~CommandLineParser() {
  for (CommandLineAction* p : this->_actions) {
    delete p;
  }
}

void CommandLineParser::_init(const CommandLineParserOptions& options) {
  _options = options;
  toolFilename = options.toolFilename;
  toolDescription = options.toolDescription;
}

const std::vector<CommandLineAction*>& CommandLineParser::actions() const {
  return _actions;
}

void CommandLineParser::addAction(CommandLineAction* action) {
  action->_buildParser();
  this->_actions.push_back(action);
  this->_actionsByName[action->actionName] = action;
}

CommandLineAction* CommandLineParser::getAction(const std::string& actionName) {
  CommandLineAction* action = this->tryGetAction(actionName);
  if (action == nullptr) {
    throw CommandLineError(ACTION_UNDEFINED, "The action \"" + actionName + "\" was not defined");
  }
  return action;
}
CommandLineAction* CommandLineParser::tryGetAction(const std::string& actionName) {
  if (this->_actionsByName.find(actionName) != this->_actionsByName.end()) {
    return this->_actionsByName.at(actionName);
  }
  return nullptr;
}

void CommandLineParser::onExecute() {
  if (this->selectedAction == nullptr) {
    return;
  }
  this->selectedAction->_execute();
}

void CommandLineParser::execute(int argc, char** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.push_back(argv[i]);
  }
  this->execute(args);
}

void CommandLineParser::execute(int argc, wchar_t** argv) {
  std::vector<std::string> args;
  for (int i = 1; i < argc; i++) {
    args.push_back(commandline::string::w2a(argv[i]));
  }
  this->execute(args);
}

void CommandLineParser::_validateDefinitions() const {
  if (this->_remainder != nullptr && this->_actions.size() > 0) {
    // This is apparently not supported by argparse
    throw CommandLineError(REMAINDER_DEFINED, "defineCommandLineRemainder() cannot be called for a CommandLineParser with actions");
  }
}

void CommandLineParser::execute(const std::vector<std::string>& args) {
  if (this->_executed) {
    throw CommandLineError(EXECUTE_AGAIN, "execute() was already called for this parser instance");
  }
  this->_executed = true;

  this->_validateDefinitions();

  size_t length = args.size();
  if (length == 0) {
    std::cout << this->renderHelpText() << std::endl;
    return;
  }

  std::vector<std::string> mainArgs;
  std::vector<std::string> actionArgs;
  size_t i = 0;
  for (; i < length; i++) {
    if (args[i][0] == '-') {
      mainArgs.push_back(args[i]);
    } else {
      if (i == 0) {
        break;
      }
      try {
        this->getFlagParameter(args[i - 1]);
        if (args[i] == "true" || args[i] == "false" || args[i] == "1" || args[i] == "0") {
          continue;
        } else {
          break;
        }
      } catch (const CommandLineError& err) {
        if (err.code() == PARAMETER_UNDEFINED) {
          break;
        } else if (err.code() == PARAMETER_TYPE_ERROR) {
          continue;
        } else {
          throw err;
        }
      }
    }
  }

  if (commandline::string::indexOf(mainArgs, "-h") != -1 || commandline::string::indexOf(mainArgs, "--help") != -1) {
    std::cout << this->renderHelpText() << std::endl;
    return;
  }

  if (this->_remainder != nullptr) {
    // std::vector<std::string> remain;
    // for (size_t x = i; x < length; x++) {
    //   remain.push_back(args[x]);
    // }
    // this->_remainder->_setValue(remain);

    for (size_t x = i; x < length; x++) {
      mainArgs.push_back(args[x]);
    }
    this->_processArgs(mainArgs);
    this->onExecute();
  } else {
    if (i == length) {
      // throw CommandLineError(ACTION_UNKNOWN, "Unrecognized action");
      this->_processArgs(mainArgs);
      this->onExecute();
      return;
    }

    try {
      this->selectedAction = this->getAction(args[i]);
      i++;
    } catch (const CommandLineError&) {
      throw CommandLineError(ACTION_UNDEFINED, "Unrecognized action");
    }

    for (; i < length; i++) {
      actionArgs.push_back(args[i]);
    }

    if (commandline::string::indexOf(actionArgs, "-h") != -1 || commandline::string::indexOf(actionArgs, "--help") != -1) {
      std::cout << this->selectedAction->renderHelpText(this->toolFilename) << std::endl;
      return;
    }

    this->_processArgs(mainArgs);
    this->selectedAction->_processArgs(actionArgs);
    this->onExecute();
  }
}

std::string CommandLineParser::renderHelpText() const {
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
  std::string usage = "usage: " + this->toolFilename + " ";
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

  usage += shortOptions + (this->_remainder == nullptr ? (" <command> ...") : (" " + this->_remainder->argumentName)) + EOL + EOL + this->toolDescription + EOL + EOL;

  std::string actionPart = "";
  if (this->_actions.size() > 0) {
    actionPart = "Commands:" + EOL;
    const size_t len = indent;

    for (const CommandLineAction* a : this->_actions) {
      std::string front = repeat(" ", 2) + a->actionName;
      std::string space = len < front.length() + 1 ? EOL + repeat(" ", len) : repeat(" ", len - front.length());
      actionPart += front + space + a->summary + EOL;
    }
  }

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

  std::string foot = "";
  if (this->_remainder == nullptr && this->_actions.size() > 0) {
    foot = EOL + "For detailed help about a specific command, use: example <command> -h";
  }

  return usage + actionPart + EOL + optionPart + foot;
}

}
