#ifndef __COMMAND_LINE_ACTION_HPP__
#define __COMMAND_LINE_ACTION_HPP__

#include <regex>
#include "CommandLineParameterProvider.hpp"
#include "CommandLineDefinition.hpp"

namespace commandline {

class CommandLineAction : public CommandLineParameterProvider {
 private:
  static std::regex _actionNameRegExp;
 protected:
  void _init(const CommandLineActionOptions&);
 public:
  std::string actionName;
  std::string summary;
  std::string documentation;
  CommandLineAction();
  CommandLineAction(const CommandLineActionOptions&);
  virtual ~CommandLineAction();

  CommandLineAction(const CommandLineAction&) = delete;
  CommandLineAction(CommandLineAction&&) = default;
  CommandLineAction& operator=(const CommandLineAction&) = delete;
  CommandLineAction& operator=(CommandLineAction&&) = default;

  static void* operator new(size_t);
  static void* operator new[](size_t);
  static void operator delete(void*) noexcept;
  static void operator delete[](void*) noexcept;

  virtual void _processArgs(const std::vector<std::string>&);

  virtual void onDefineParameters() = 0;
  virtual void onExecute() = 0;

  virtual std::string renderHelpText(const std::string&) const;

  void _buildParser();
  void _execute();
};

}

#endif
