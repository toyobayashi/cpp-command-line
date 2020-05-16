#include "commandline/DynamicCommandLineAction.hpp"

namespace commandline {

DynamicCommandLineAction::DynamicCommandLineAction(): CommandLineAction() {}
DynamicCommandLineAction::DynamicCommandLineAction(const CommandLineActionOptions& options): CommandLineAction(options) {}

void DynamicCommandLineAction::onDefineParameters() {}
void DynamicCommandLineAction::onExecute() {}

}
