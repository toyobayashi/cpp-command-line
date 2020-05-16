#ifndef __DYNAMIC_COMMAND_LINE_ACTION_HPP__
#define __DYNAMIC_COMMAND_LINE_ACTION_HPP__

#include "CommandLineAction.hpp"

namespace commandline {

class DynamicCommandLineAction : public CommandLineAction {
 public:
  DynamicCommandLineAction();
  DynamicCommandLineAction(const CommandLineActionOptions&);
 protected:
  void onDefineParameters();
  void onExecute();
};

}

#endif
