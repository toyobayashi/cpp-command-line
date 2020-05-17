#ifndef __COMMAND_LINE_PARAMETER_HPP__
#define __COMMAND_LINE_PARAMETER_HPP__

#include <regex>
#include "CommandLineDefinition.hpp"

namespace commandline {
  class CommandLineParameter {
   private:
    static std::regex _longNameRegExp;
    static std::regex _shortNameRegExp;
    static std::regex _environmentVariableRegExp;
    
    bool _hasValue;
   protected:
    // std::string _parserKey;

   public:
    const std::string longName;
    const std::string shortName;
    const std::string description;
    const bool required;
    const std::string environmentVariable;

    CommandLineParameter(const BaseCommandLineDefinition&);
    virtual ~CommandLineParameter();

    CommandLineParameter(const CommandLineParameter&) = default;
    CommandLineParameter(CommandLineParameter&&) = default;
    CommandLineParameter& operator=(const CommandLineParameter&) = default;
    CommandLineParameter& operator=(CommandLineParameter&&) = default;

    virtual void _setValue() = 0;
    virtual void _setValue(bool) = 0;
    virtual void _setValue(int64_t) = 0;
    virtual void _setValue(const std::string&) = 0;
    virtual void _setValue(const std::vector<std::string>&) = 0;

    bool hasValue() const;
    void setHasValue();

    virtual void _getSupplementaryNotes(std::vector<std::string>&) const;
    virtual CommandLineParameterKind kind() const = 0;
    virtual void appendToArgList(std::vector<std::string>&) const = 0;
   protected:
    void reportInvalidData(bool) const;
    void reportInvalidData(int64_t) const;
    void reportInvalidData(const std::string&) const;
    void reportInvalidData(const std::vector<std::string>&) const;

    void validateDefaultValue(bool) const;

    std::string formatStringArray(const std::vector<std::string>& arr) const;
  };

  class CommandLineParameterWithArgument : public CommandLineParameter {
   private:
    static std::regex _invalidArgumentNameRegExp;

   public:
    const std::string argumentName;

    CommandLineParameterWithArgument(const BaseCommandLineDefinitionWithArgument&);
    virtual ~CommandLineParameterWithArgument();

    CommandLineParameterWithArgument(const CommandLineParameterWithArgument&) = default;
    CommandLineParameterWithArgument(CommandLineParameterWithArgument&&) = default;
    CommandLineParameterWithArgument& operator=(const CommandLineParameterWithArgument&) = default;
    CommandLineParameterWithArgument& operator=(CommandLineParameterWithArgument&&) = default;
  };

  class CommandLineChoiceParameter : public CommandLineParameter {
   private:
    std::string _value;

   public:
    const std::vector<std::string> alternatives;
    std::string defaultValue;

    CommandLineChoiceParameter(const CommandLineChoiceDefinition&);

    CommandLineParameterKind kind() const;

    void _setValue();
    void _setValue(bool);
    void _setValue(int64_t);
    void _setValue(const std::string&);
    void _setValue(const std::vector<std::string>&);

    void _getSupplementaryNotes(std::vector<std::string>&) const;

    void appendToArgList(std::vector<std::string>&) const;

    std::string formatAlternatives() const;

    const std::string& value() const;
  };

  class CommandLineFlagParameter : public CommandLineParameter {
   private:
    bool _value;

   public:
    const bool defaultValue;
    CommandLineFlagParameter(const CommandLineFlagDefinition&);

    CommandLineParameterKind kind() const;

    void _setValue();
    void _setValue(bool);
    void _setValue(int64_t);
    void _setValue(const std::string&);
    void _setValue(const std::vector<std::string>&);

    void _getSupplementaryNotes(std::vector<std::string>&) const;

    void appendToArgList(std::vector<std::string>&) const;

    bool value() const;
  };

  class CommandLineIntegerParameter : public CommandLineParameterWithArgument {
   private:
    int64_t _value;

   public:
    const int64_t defaultValue;

    CommandLineIntegerParameter(const CommandLineIntegerDefinition&);

    CommandLineParameterKind kind() const;

    void _setValue();
    void _setValue(bool);
    void _setValue(int64_t);
    void _setValue(const std::string&);
    void _setValue(const std::vector<std::string>&);

    void _getSupplementaryNotes(std::vector<std::string>&) const;

    void appendToArgList(std::vector<std::string>&) const;

    int64_t value() const;
  };

  class CommandLineStringParameter : public CommandLineParameterWithArgument {
   private:
    std::string _value;

   public:
    const std::string defaultValue;

    CommandLineStringParameter(const CommandLineStringDefinition&);

    CommandLineParameterKind kind() const;

    void _setValue();
    void _setValue(bool);
    void _setValue(int64_t);
    void _setValue(const std::string&);
    void _setValue(const std::vector<std::string>&);

    void _getSupplementaryNotes(std::vector<std::string>&) const;

    void appendToArgList(std::vector<std::string>&) const;

    const std::string& value() const;
  };

  class CommandLineStringListParameter : public CommandLineParameterWithArgument {
   private:
    std::vector<std::string> _values;

   public:

    CommandLineStringListParameter(const CommandLineStringListDefinition&);

    CommandLineParameterKind kind() const;

    void _setValue();
    void _setValue(bool);
    void _setValue(int64_t);
    void _setValue(const std::string&);
    void _setValue(const std::vector<std::string>&);

    void appendToArgList(std::vector<std::string>&) const;

    const std::vector<std::string>& values() const;
  };
}

#endif
