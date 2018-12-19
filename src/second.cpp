#include "second.hpp"

#include <cstring>

namespace sirius {

void RemoveDups(char *inOut) {
  constexpr int MAX_STR_LEN = 32;

  if (inOut == nullptr)
    return;
  char buf[MAX_STR_LEN]{};

  int inChar = 0;
  int outChar = 0;
  char prevChar = 0;

  while (inOut[inChar] != 0) {
    if (inOut[inChar] != prevChar) {
      buf[outChar] = prevChar = inOut[inChar];
      ++outChar;
    }
    ++inChar;
  };

  strcpy(inOut, buf);
}

}