#pragma once

#include <cstdint>

namespace sirius {

// Problem 1:
// Write a function that prints signed integer as it's binary representation without using library functions

using PrintableInt = int64_t;
constexpr int BUF_SIZE = sizeof(PrintableInt) * 8 + 1; //+1 for trailing \0

/**
 * @param out length of the out must be at least BUF_SIZE bytes!
 */
void ConvertToBinStr(PrintableInt in, char *out);

}
