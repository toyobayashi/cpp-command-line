#include "commandline/CommandLineError.hpp"

namespace commandline {
  CommandLineError::~CommandLineError() {}

  CommandLineError::CommandLineError(CommandLineErrorCode code, const std::string& message)
    :_code(code), _message(message) {}
  
  const char* CommandLineError::what() const noexcept { return _message.c_str(); }

  CommandLineErrorCode CommandLineError::code() const { return _code; }
}
