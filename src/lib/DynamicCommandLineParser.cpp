#include "commandline/DynamicCommandLineParser.hpp"

namespace commandline {

DynamicCommandLineParser::DynamicCommandLineParser(): CommandLineParser() {}
DynamicCommandLineParser::DynamicCommandLineParser(const CommandLineParserOptions& options): CommandLineParser(options) {}

}
