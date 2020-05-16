#include "commandline/CommandLineParameter.hpp"
#include "commandline/CommandLineError.hpp"

namespace commandline {
  std::regex CommandLineParameterWithArgument::_invalidArgumentNameRegExp("[^A-Z_0-9]");

  CommandLineParameterWithArgument::~CommandLineParameterWithArgument() {}

  CommandLineParameterWithArgument::CommandLineParameterWithArgument(const BaseCommandLineDefinitionWithArgument& definition):
    CommandLineParameter(definition), argumentName(definition.argumentName) {
    if (definition.argumentName == "") {
      throw CommandLineError(INVALID_NAME, "The argument name cannot be an empty string. (For the default name, specify undefined.)");
    }

    std::string upper;
    for (size_t i = 0; i < definition.argumentName.length(); i++) {
      if (definition.argumentName[i] >= 97 && definition.argumentName[i] <= 122) {
        upper += definition.argumentName[i] - 32;
      } else {
        upper += definition.argumentName[i];
      }
    }
    if (upper != definition.argumentName) {
      throw CommandLineError(INVALID_NAME, "Invalid name: \"" + definition.argumentName + "\". The argument name must be all upper case.");
    }

    std::smatch sm;

    if (std::regex_search(definition.argumentName, sm, CommandLineParameterWithArgument::_invalidArgumentNameRegExp)) {
      throw CommandLineError(INVALID_NAME, "The argument name: \"" + definition.argumentName + "\" contains an invalid character \"" + sm[0].str() + "\". Only upper-case letters, numbers, and underscores are allowed.");
    }
  }
}
