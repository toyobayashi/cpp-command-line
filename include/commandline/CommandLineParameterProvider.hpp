#ifndef __COMMAND_LINE_PARAMETER_PROVIDER_HPP__
#define __COMMAND_LINE_PARAMETER_PROVIDER_HPP__

#include <map>
#include "CommandLineParameter.hpp"
#include "CommandLineDefinition.hpp"

namespace commandline {

class CommandLineParameterProvider {
 private:
  std::vector<CommandLineParameter*> _parameters;
  std::map<std::string, CommandLineParameter*> _parametersByLongName;
  std::map<std::string, CommandLineParameter*> _parametersByShortName;
  
  CommandLineParameter* _getParameter(const std::string&) const;
  const CommandLineParameter* _getParameter(const std::string&, CommandLineParameterKind) const;
  CommandLineParameter* _tryGetParameter(const std::string&) const;
  void _defineParameter(CommandLineParameter*);
 protected:
  static std::string _defaultValueToString(const CommandLineParameter*);
  static std::string _kindToString(CommandLineParameterKind);
  static std::string _kindToString(const CommandLineParameter*);
  virtual void _processArgs(const std::vector<std::string>&);
 public:
  CommandLineParameterProvider();
  virtual ~CommandLineParameterProvider();

  const std::vector<CommandLineParameter*>& parameters() const;

  CommandLineParameterProvider(const CommandLineParameterProvider&) = delete;
  CommandLineParameterProvider(CommandLineParameterProvider&&) = default;
  CommandLineParameterProvider& operator=(const CommandLineParameterProvider&) = delete;
  CommandLineParameterProvider& operator=(CommandLineParameterProvider&&) = default;

  CommandLineChoiceParameter* defineChoiceParameter(const CommandLineChoiceDefinition&);
  const CommandLineChoiceParameter* getChoiceParameter(const std::string&) const;
  CommandLineFlagParameter* defineFlagParameter(const CommandLineFlagDefinition&);
  const CommandLineFlagParameter* getFlagParameter(const std::string&) const;

  CommandLineIntegerParameter* defineIntegerParameter(const CommandLineIntegerDefinition&);
  const CommandLineIntegerParameter* getIntegerParameter(const std::string&) const;

  CommandLineStringParameter* defineStringParameter(const CommandLineStringDefinition&);
  const CommandLineStringParameter* getStringParameter(const std::string&) const;

  CommandLineStringListParameter* defineStringListParameter(const CommandLineStringListDefinition&);
  const CommandLineStringListParameter* getStringListParameter(const std::string&) const;

  // virtual std::string renderHelpText() const = 0;

  // virtual void onDefineParameters() = 0;
};

}

#endif
