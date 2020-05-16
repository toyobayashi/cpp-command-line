#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#else
#include <clocale>
#endif

#include "StringUtil.hpp"
#include <cstring>
#include <cstdlib>

namespace commandline {

namespace string {

int indexOf(const std::vector<std::string>& arr, const std::string& v) {
  for (int i = 0; i < (int)arr.size(); i++) {
    if (v == arr[i]) {
      return i;
    }
  }
  return -1;
}

std::vector<std::wstring> wsplit(const std::wstring& self, const std::wstring& separator, int limit) {
  std::wstring copy = self;
  wchar_t* copyBuf = new wchar_t[copy.size() + 1];
  memset(copyBuf, 0, (copy.size() + 1) * sizeof(wchar_t));
  wcscpy(copyBuf, copy.c_str());
#ifdef _MSC_VER
  wchar_t* tokenPtr = _wcstok(copyBuf, separator.c_str());
#else
  wchar_t* buffer;
  wchar_t* tokenPtr = wcstok(copyBuf, separator.c_str(), &buffer);
#endif
  std::vector<std::wstring> res;
  while (tokenPtr != NULL && (limit == -1 ? true : ((int)res.size()) < limit)) {
    res.push_back(tokenPtr);
#ifdef _MSC_VER
    tokenPtr = _wcstok(NULL, separator.c_str());
#else
    tokenPtr = wcstok(NULL, separator.c_str(), &buffer);
#endif
  }
  if (copyBuf[copy.size() - 1] == L'\0') {
    res.push_back(L"");
  }
  delete[] copyBuf;
  return res;
}

std::vector<std::string> split(const std::string& self, const std::string& separator, int limit) {
  std::string copy = self;
  char* copyBuf = new char[copy.size() + 1];
  memset(copyBuf, 0, (copy.size() + 1) * sizeof(char));
  strcpy(copyBuf, copy.c_str());
#ifdef _MSC_VER
  char* tokenPtr = strtok(copyBuf, separator.c_str());
#else
  char* buffer;
  char* tokenPtr = strtok_r(copyBuf, separator.c_str(), &buffer);
#endif
  std::vector<std::string> res;
  while (tokenPtr != NULL && (limit == -1 ? true : ((int)res.size()) < limit)) {
    res.push_back(tokenPtr);
#ifdef _MSC_VER
    tokenPtr = strtok(NULL, separator.c_str());
#else
    tokenPtr = strtok_r(NULL, separator.c_str(), &buffer);
#endif
  }
  if (copyBuf[copy.size() - 1] == '\0') {
    res.push_back("");
  }
  delete[] copyBuf;
  return res;
}

std::string w2a(const std::wstring& wstr) {
#ifdef _WIN32
  int len = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, NULL, NULL);
  if (len == -1) {
    return "";
  }
  char* buf = new char[len];
  WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, buf, len, NULL, NULL);
  std::string res(buf);
  delete[] buf;
  return res;
#else
  std::string target_locale = "en_US.utf8";
  const char* c_locale = std::setlocale(LC_CTYPE, nullptr);
  std::string locale(c_locale ? c_locale : "");

  if (locale != target_locale) {
    std::setlocale(LC_CTYPE, target_locale.c_str());
  }

  size_t len = std::wcstombs(nullptr, wstr.c_str(), 0);
  char* buf = new char[len + 1];
  memset(buf, 0, (len + 1) * sizeof(char));
  std::wcstombs(buf, wstr.c_str(), len + 1);
  std::string res(buf);
  delete[] buf;

  if (locale != "") {
    std::setlocale(LC_CTYPE, locale.c_str());
  }
  return res;
#endif
}

}

}
