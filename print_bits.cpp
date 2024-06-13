// print_bits.cpp
// (c) Reza Manoochehrian JUN-2024

#include "print_bits.hpp"

//
// skip_bytes:     0: no skipping, N: skip first N bytes
// read_bytes:     0: read all, N: read up to N bytes
// line_break:     0: no line break, N: line break every N bytes
// byte_separator: string, default: no delimiter
// line_numbers:   default: no, any value interpreted as yes
//

void print_bits
(
  PrintMode m
  , std::ostream& o
  , const std::vector<unsigned char>& v
  , const unsigned long skip_bytes
  , const unsigned long read_bytes
  , const unsigned long line_break
  , const bool line_numbers
  , const char byte_separator
)
{
  unsigned long count_running_bytes{ 0 };
  unsigned long count_printed_bytes{ 0 };
  unsigned long count_skipped_bytes{ 0 };
  unsigned long count_printed_lines{ 0 };

  for (const unsigned char byte : v)
  {
    count_running_bytes++;
    if (skip_bytes > 0 && count_skipped_bytes < skip_bytes)
    {
      count_skipped_bytes++;
      continue;
    }

    if (read_bytes == 0 || (count_running_bytes - count_skipped_bytes - 1 < read_bytes))
    {
      if (m == PrintMode::bits)
      {
        for (int i = 7; i >= 0; --i)
        {
          o << ((byte >> i) & 1);
        }
      }
      else if (m == PrintMode::bytes_pretty_binary)
      {
        o << (byte ? '*' : ' ');
      }
      else
      {
        o << byte;
      }

      o << byte_separator;

      if (++count_printed_bytes == line_break)
      {
        count_printed_bytes = 0;

        if (line_numbers)
        {
          o << " : " << ++count_printed_lines;
        }

        o << std::endl;
      }
    }
  }
}
