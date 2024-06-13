// print_bits.h
// (c) Reza Manoochehrian JUN-2024

#include <fstream>
#include <vector>
#include <string>

enum class PrintMode { bits, bytes_raw, bytes_pretty_binary };

void print_bits
(
  PrintMode
  , std::ostream&
  , const std::vector<unsigned char>&
  , const unsigned long skip_bytes // 0: no skipping, N: skip first N bytes
  , const unsigned long read_bytes // 0: read all, N: read up to N bytes
  , const unsigned long line_break // 0: no line break, N: line break every N bytes
  , const bool = false
  , const char byte_separator = ' '
);
