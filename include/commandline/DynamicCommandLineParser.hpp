#ifndef __DYNAMIC_COMMAND_LINE_PARSER_HPP__
#define __DYNAMIC_COMMAND_LINE_PARSER_HPP__

#include "CommandLineParser.hpp"

namespace commandline {

class DynamicCommandLineParser : public CommandLineParser {
 public:
  DynamicCommandLineParser();
  DynamicCommandLineParser(const CommandLineParserOptions&);
};

}

#endif
