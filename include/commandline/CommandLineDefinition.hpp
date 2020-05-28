#ifndef __COMMAND_LINE_DEFINITION_HPP__
#define __COMMAND_LINE_DEFINITION_HPP__

#include <string>
#include <vector>
#include <cstdint>

namespace commandline {
  struct BaseCommandLineDefinition {
    std::string parameterLongName = "";
    std::string parameterShortName = "";
    std::string description = "";
    bool required = false;
    std::string environmentVariable = "";
  };

  struct BaseCommandLineDefinitionWithArgument : BaseCommandLineDefinition {
    std::string argumentName = "";
  };

  struct CommandLineChoiceDefinition : BaseCommandLineDefinition {
    std::vector<std::string> alternatives = {};
    std::string defaultValue = "";
  };

  struct CommandLineFlagDefinition : BaseCommandLineDefinition {
    bool defaultValue = false;
  };

  struct CommandLineIntegerDefinition : BaseCommandLineDefinitionWithArgument {
    int64_t defaultValue = 0;
  };

  struct CommandLineStringDefinition : BaseCommandLineDefinitionWithArgument {
    std::string defaultValue = "";
  };

  struct CommandLineStringListDefinition : BaseCommandLineDefinitionWithArgument {
  };

  struct CommandLineRemainderDefinition {
    std::string argumentName = "...";
    std::string description = "";
  };

  struct CommandLineActionOptions {
    std::string actionName = "";
    std::string summary = "";
    std::string documentation = "";
  };

  struct CommandLineParserOptions {
    std::string toolFilename = "";
    std::string toolDescription = "";
  };

  typedef enum CommandLineParameterKind {
    /** Indicates a CommandLineChoiceParameter */
    Choice,
    /** Indicates a CommandLineFlagParameter */
    Flag,
    /** Indicates a CommandLineIntegerParameter */
    Integer,
    /** Indicates a CommandLineStringParameter */
    String,
    /** Indicates a CommandLineStringListParameter */
    StringList
  } CommandLineParameterKind;
}

#endif
