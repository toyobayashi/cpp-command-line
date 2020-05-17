#include <stdio.h>

#include "commandline/commandline.hpp"

#include <iostream>
#include <memory>
#include <exception>

#include "cmocha/cmocha.h"

using namespace commandline;

class TestAction : public CommandLineAction {
 public:
  bool done;
 private:
  CommandLineFlagParameter* _flag;
 public:
  TestAction(): CommandLineAction(), done(false), _flag(nullptr) {
    CommandLineActionOptions o;
    o.actionName = "do:the-job";
    o.summary = "does the job";
    o.documentation = "a longer description";
    this->_init(o);
  }
 protected:
  void onExecute() {
    std::cout << this->_flag->value() << std::endl;
    done = true;
  }

  void onDefineParameters() {
    CommandLineFlagDefinition d;
    d.parameterLongName = "--flag";
    d.description = "The flag";
    _flag = defineFlagParameter(d);
  }
};

class TestCommandLine : public CommandLineParser {
  public:
  TestCommandLine(): CommandLineParser() {
    CommandLineParserOptions o;
    o.toolFilename = "example";
    o.toolDescription = "An example project";
    this->_init(o);
    this->addAction(new TestAction());
  }
};

static int executes_an_action() {
  TestCommandLine commandLineParser;
  commandLineParser.execute({ "do:the-job" });
  expect(commandLineParser.selectedAction != nullptr);
  expect(commandLineParser.selectedAction->actionName == "do:the-job");

  TestAction* action = static_cast<TestAction*>(commandLineParser.selectedAction);
  expect(action->done == true);
  return 0;
}


static int parse_an_action() {
  CommandLineParserOptions options;
  options.toolFilename = "example";
  options.toolDescription = "An example project";
  DynamicCommandLineParser commandLineParser(options);

  CommandLineActionOptions actionOptions;
  actionOptions.actionName = "do:the-job";
  actionOptions.summary = "does the job";
  actionOptions.documentation = "a longer description";

  DynamicCommandLineAction* action = new DynamicCommandLineAction(actionOptions);
  commandLineParser.addAction(action);

  CommandLineFlagDefinition d;
  d.parameterLongName = "--flag";
  d.description = "The flag";
  action->defineFlagParameter(d);
  try {
    commandLineParser.execute({ "do:the-job", "--flag" });

    expect(commandLineParser.selectedAction == action);
    const CommandLineFlagParameter* retrievedParameter = action->getFlagParameter("--flag");
    expect(retrievedParameter->value() == true);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }
  return 0;
}

static DynamicCommandLineParser* createParser() {
  CommandLineParserOptions options;
  options.toolFilename = "example";
  options.toolDescription = "An example project";
  DynamicCommandLineParser* commandLineParser = new DynamicCommandLineParser(options);

  CommandLineFlagDefinition d;
  d.parameterLongName = "--global-flag";
  d.parameterShortName = "-g";
  d.description = "A flag that affects all actions";

  commandLineParser->defineFlagParameter(d);

  CommandLineActionOptions actionOptions;
  actionOptions.actionName = "do:the-job";
  actionOptions.summary = "does the job";
  actionOptions.documentation = "a longer description";
  DynamicCommandLineAction* action = new DynamicCommandLineAction(actionOptions);
  commandLineParser->addAction(action);

  CommandLineChoiceDefinition choiceDef;
  choiceDef.parameterLongName = "--choice";
  choiceDef.parameterShortName = "-c";
  choiceDef.description = "A choice";
  choiceDef.alternatives = { "one", "two", "three", "default" };
  choiceDef.environmentVariable = "ENV_CHOICE";
  action->defineChoiceParameter(choiceDef);

  CommandLineChoiceDefinition choiceDef2;
  choiceDef2.parameterLongName = "--choice-with-default";
  choiceDef2.description = "A choice with a default";
  choiceDef2.alternatives = { "one", "two", "three", "default" };
  choiceDef2.environmentVariable = "ENV_CHOICE";
  choiceDef2.defaultValue = "default";
  action->defineChoiceParameter(choiceDef2);

  CommandLineFlagDefinition flagDef;
  flagDef.parameterLongName = "--flag";
  flagDef.parameterShortName = "-f";
  flagDef.description = "A flag";
  flagDef.environmentVariable = "ENV_FLAG";
  action->defineFlagParameter(flagDef);

  CommandLineIntegerDefinition intDef;
  intDef.parameterLongName = "--integer";
  intDef.parameterShortName = "-i";
  intDef.description = "An integer";
  intDef.argumentName = "NUMBER";
  intDef.environmentVariable = "ENV_INTEGER";
  action->defineIntegerParameter(intDef);

  CommandLineIntegerDefinition intDef2;
  intDef2.parameterLongName = "--integer-with-default";
  intDef2.description = "An integer with a default";
  intDef2.argumentName = "NUMBER";
  intDef2.environmentVariable = "ENV_INTEGER";
  intDef2.defaultValue = 123;
  action->defineIntegerParameter(intDef2);

  CommandLineIntegerDefinition intDef3;
  intDef3.parameterLongName = "--integer-required";
  intDef3.description = "An integer";
  intDef3.argumentName = "NUMBER";
  intDef3.required = true;
  action->defineIntegerParameter(intDef3);

  CommandLineStringDefinition strDef;
  strDef.parameterLongName = "--string";
  strDef.parameterShortName = "-s";
  strDef.description = "A string";
  strDef.argumentName = "TEXT";
  strDef.environmentVariable = "ENV_STRING";
  action->defineStringParameter(strDef);

  CommandLineStringDefinition strDef2;
  strDef2.parameterLongName = "--string-with-default";
  strDef2.description = "A string with a default";
  strDef2.argumentName = "TEXT";
  strDef2.environmentVariable = "ENV_STRING";
  strDef2.defaultValue = "123";
  action->defineStringParameter(strDef2);

  CommandLineStringListDefinition strlistDef;
  strlistDef.parameterLongName = "--string-list";
  strlistDef.parameterShortName = "-l";
  strlistDef.description = "A string list";
  strlistDef.argumentName = "LIST";
  strlistDef.environmentVariable = "ENV_STRING_LIST";
  action->defineStringListParameter(strlistDef);

  return commandLineParser;
}

static int parses_an_input_with_ALL_parameters() {
  std::unique_ptr<DynamicCommandLineParser> commandLineParser(createParser());
  CommandLineAction* action = commandLineParser->getAction("do:the-job");
  std::vector<std::string> args = {
    "--global-flag",
    "do:the-job",
    "--choice", "two",
    "--flag",
    "--integer", "123",
    "--integer-required", "321",
    "--string", "hello",
    "--string-list", "first",
    "--string-list", "second"
  };
  try {
    commandLineParser->execute(args);
    expect(commandLineParser->selectedAction == action);
    expect(commandLineParser->getFlagParameter("--global-flag")->value() == true);
    expect(action->getChoiceParameter("--choice")->value() == "two");
    expect(action->getChoiceParameter("--choice-with-default")->value() == "default");
    expect(action->getFlagParameter("--flag")->value() == true);
    expect(action->getIntegerParameter("--integer")->value() == 123);
    expect(action->getIntegerParameter("--integer-with-default")->value() == 123);
    expect(action->getIntegerParameter("--integer-required")->value() == 321);
    expect(action->getStringParameter("--string")->value() == "hello");
    expect(action->getStringParameter("--string-with-default")->value() == "123");
    const std::vector<std::string>& values = action->getStringListParameter("--string-list")->values();
    expect(values[0] == "first");
    expect(values[1] == "second");
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  return 0;
}

static int parses_an_input_with_NO_parameters() {
  std::unique_ptr<DynamicCommandLineParser> commandLineParser(createParser());
  CommandLineAction* action = commandLineParser->getAction("do:the-job");
  std::vector<std::string> args = {
    "do:the-job",
    "--integer-required", "-123"
  };
  try {
    commandLineParser->execute(args);
    expect(commandLineParser->selectedAction == action);
    expect(commandLineParser->getFlagParameter("--global-flag")->value() == false);
    expect(action->getChoiceParameter("--choice")->value() == action->getChoiceParameter("--choice")->alternatives[0]);
    expect(action->getChoiceParameter("--choice-with-default")->value() == "default");
    expect(action->getFlagParameter("--flag")->value() == false);
    expect(action->getIntegerParameter("--integer")->value() == 0);
    expect(action->getIntegerParameter("--integer-with-default")->value() == 123);
    expect(action->getIntegerParameter("--integer-required")->value() == -123);
    expect(action->getStringParameter("--string")->value() == "");
    expect(action->getStringParameter("--string-with-default")->value() == "123");
    const std::vector<std::string>& values = action->getStringListParameter("--string-list")->values();
    expect(values.size() == 0);
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  return 0;
}

static int test_action_help() {
  std::unique_ptr<DynamicCommandLineParser> commandLineParser(createParser());
  try {
    commandLineParser->execute({ "do:the-job", "-h" });
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  return 0;
}

static int test_global_help() {
  std::unique_ptr<DynamicCommandLineParser> commandLineParser(createParser());
  try {
    commandLineParser->execute({});
  } catch (const std::exception& err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  return 0;
}

int main() {
  int r = 0;
  int ret = 0;
  r = describe("CommandLineParser", 
    executes_an_action
  );
  if (r != 0) {
    ret = r;
  }

  r = describe("DynamicCommandLineParser", 
    parse_an_action
  );
  if (r != 0) {
    ret = r;
  }
  r = describe("CommandLineParameter", 
    parses_an_input_with_ALL_parameters,
    parses_an_input_with_NO_parameters,
    test_global_help,
    test_action_help
  );
  if (r != 0) {
    ret = r;
  }
  return ret;
}
