#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#endif

#include "EnvironmentVariable.hpp"
#include "StringUtil.hpp"

#ifdef _WIN32

#else
#include <unistd.h>
#ifdef __APPLE__
#include <crt_externs.h>
#define environ (*_NSGetEnviron())
#else
extern char** environ;
#endif
#endif

namespace commandline {

const std::map<std::string, std::string>& env() {
  static std::map<std::string, std::string> _env;
  if (_env.size() == 0) {
#ifdef _WIN32
    wchar_t* environment = GetEnvironmentStringsW();
    if (environment == nullptr) return _env;
    wchar_t* p = environment;
    while (*p) {
      if (*p == L'=') {
        p += wcslen(p) + 1;
        continue;
      }
      std::wstring e = p;
      std::vector<std::wstring> keyvalue = commandline::string::wsplit(e, L"=");
      _env[commandline::string::w2a(keyvalue[0])] = commandline::string::w2a(keyvalue[1]);

      p += wcslen(p) + 1;
    }
    FreeEnvironmentStringsW(environment);
#else
    int env_size = 0;
    while (environ[env_size]) {
      std::string e = environ[env_size];
      std::vector<std::string> keyvalue = commandline::string::split(e, "=");
      _env[keyvalue[0]] = keyvalue[1];
      env_size++;
    }
#endif
  }
  return _env;
}

}
