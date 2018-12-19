#include <iostream>

#include "first.hpp"

namespace sirius {

void ConvertToBinStr(PrintableInt in, char *out) {
  if (out == nullptr)
    return;
  constexpr char ONE = '1';
  constexpr char ZERO = '0';
  out[BUF_SIZE - 1] = 0; // terminate c-string
  for (int i = BUF_SIZE - 2; i > -1; --i) {
    if (in & 1) {
      out[i] = ONE;
    } else {
      out[i] = ZERO;
    }
    in >>= 1;
  }
}

}