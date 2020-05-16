#ifndef __COMMAND_LINE_ERROR_HPP__
#define __COMMAND_LINE_ERROR_HPP__

#include <string>
#include <exception>

namespace commandline {
  typedef enum CommandLineErrorCode {
    INVALID_NAME = 1,
    EMPTY_ALTERNATIVE_LIST,
    ERROR_ALTERNATIVE_DEFAULT_VALUE,
    INVALID_ENV_VALUE,
    INVALID_ENV,
    INVALID_VALUE,
    UNEXPECTED_DATA,
    UNEXPECTED_DEFAULT_VALUE,
    PARAMETER_UNDEFINED,
    PARAMETER_TYPE_ERROR,
    PARAMETER_TYPE_UNKNOWN,
    VALUE_REQUIRED,
    ACTION_UNKNOWN,
    ACTION_UNDEFINED,
    EXECUTE_AGAIN
  } CommandLineErrorCode;
  class CommandLineError : public std::exception {
   private:
    CommandLineErrorCode _code;
    std::string _message;
   public:
    virtual ~CommandLineError();
    CommandLineError(CommandLineErrorCode, const std::string&);
    const char* what() const noexcept;
    CommandLineErrorCode code() const;
  };
}

#endif
