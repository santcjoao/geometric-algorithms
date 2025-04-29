#include "util/Exception.h"
#include <cstdio>

namespace tcii::cg
{ // begin namespace tcii::cg

std::string
errorMessage(const char* format, va_list args)
{
  constexpr auto maxLen = 1024;
  char buffer[maxLen];

  vsnprintf(buffer, maxLen, format, args);
  return buffer;
}

std::logic_error
badInvocation(const char* c, const char* f)
{
  constexpr auto maxLen = 128;
  char buffer[maxLen];

  snprintf(buffer, maxLen, "%s::%s() invoked", c, f);
  return std::logic_error{buffer};
}

} // begin namespace tcii::cg
