#ifndef __COMMAND_LINE_PARSER_HPP__
#define __COMMAND_LINE_PARSER_HPP__

#include "CommandLineAction.hpp"
#include "CommandLineParameterProvider.hpp"
#include "CommandLineDefinition.hpp"

namespace commandline {

class CommandLineParser : public CommandLineParameterProvider {
 private:
  CommandLineParserOptions _options;
  std::vector<CommandLineAction*> _actions;
  std::map<std::string, CommandLineAction*> _actionsByName;
  bool _executed; 
 protected:
  virtual std::string _getName() const;
  virtual std::string _getDescription() const;
  virtual void onExecute();
  void _init(const CommandLineParserOptions&);
 public:
  std::string toolFilename;
  std::string toolDescription;
  CommandLineAction* selectedAction;

  CommandLineParser();
  CommandLineParser(const CommandLineParserOptions&);
  virtual ~CommandLineParser();

  CommandLineParser(const CommandLineParser&) = delete;
  CommandLineParser(CommandLineParser&&) = delete;
  CommandLineParser& operator=(const CommandLineParser&) = delete;
  CommandLineParser& operator=(CommandLineParser&&) = delete;

  const std::vector<CommandLineAction*>& actions() const;

  void addAction(CommandLineAction*);
  CommandLineAction* getAction(const std::string& actionName);
  CommandLineAction* tryGetAction(const std::string& actionName);

  void execute(int argc, char** argv);
  void execute(int argc, wchar_t** argv);
  void execute(const std::vector<std::string>&);

  virtual std::string renderHelpText() const;
};

}

#endif
