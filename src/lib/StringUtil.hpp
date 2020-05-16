#ifndef __STRING_UTIL_HPP__
#define __STRING_UTIL_HPP__

#include <string>
#include <vector>

namespace commandline {

namespace string {
  int indexOf(const std::vector<std::string>&, const std::string&);
  std::vector<std::wstring> wsplit(const std::wstring& self, const std::wstring& separator, int limit = -1);
  std::vector<std::string> split(const std::string& self, const std::string& separator, int limit = -1);
  std::string w2a(const std::wstring& wstr);
}

}

#endif
