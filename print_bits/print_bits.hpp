// print_bits.hpp
// (c) Reza Manoochehrian JUN-2024
// reza@mantam.com

#pragma once

#include <fstream>
#include <vector>

enum class PrintMode { bits, bytes_raw, bytes_pretty_binary };

void print_bits
(
  PrintMode
  , std::ostream&
  , const std::vector<unsigned char>&
  , const unsigned long skip_bytes
  , const unsigned long read_bytes
  , const unsigned long line_break
  , const bool print_line_numbers = false
  , const char byte_separator = ' '
);
