// arg.hpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#include <string>
#include <functional>

struct ArgNameValue
{
  std::string name;
  std::string description;
  std::string value;

  ArgNameValue(std::string n, std::string d) noexcept : name{ n }, description{ d } {}
  ~ArgNameValue() = default;
  ArgNameValue(const ArgNameValue&) = default;

  ArgNameValue() = delete;
  ArgNameValue& operator=(const ArgNameValue&) = delete;
  ArgNameValue(ArgNameValue&&) = delete;
  ArgNameValue& operator=(ArgNameValue&&) = delete;
};

// name=value
// 1: initialize names and description externally
// 2: pass object (r-value reference) to constructor
// 3: constructor lookups up values
// 4: and user check function is called
// 5: if any *.value is empty, print call syntax
template <typename F, int N>
struct ArgumentHandler
{
  const char * const _p_arg0;
  std::array<ArgNameValue, N> _args;
  F _checkFunction;
  std::ostream& _output;

  ~ArgumentHandler() = default;
  ArgumentHandler() = delete;
  ArgumentHandler(const ArgumentHandler&) = delete;
  ArgumentHandler& operator=(const ArgumentHandler&) = delete;
  ArgumentHandler(ArgumentHandler&&) = delete;
  ArgumentHandler& operator=(ArgumentHandler&&) = delete;

  ArgumentHandler
  (
    int argument_count
    , char** arguments
    , std::array<ArgNameValue, N>&& arg_name_description_value
    , F check_func
    , std::ostream& output = std::cout
  ) noexcept
  : _p_arg0{ arguments[ 0 ] }
  , _args{ std::move(arg_name_description_value) }
  , _checkFunction{ check_func }
  , _output{ output }
  {
    size_t count{ 0 };
    for (int i{ 1 }; i < argument_count; i++)
    {
      std::string s(arguments[i]);
      std::string::size_type pos{ s.find("=") };
      if (pos != std::string::npos)
      {
        for (int n{ 0 }; n < N; n++)
        {
          if (s.substr(0, pos) == _args[n].name)
          {
            _args[n].value = s.substr(++pos);
            count++;
            break;
          }
        }
      }
    }

    _checkFunction(_args);

    for (int n{ 0 }; n < N; n++)
    {
      if (_args[n].value.length() == 0)
      {
        print_call_syntax();
        exit(0);
      }
    }
  }

  void print_call_syntax()
  {
    _output
      << "ArgumentHandler::print_call_syntax(): missing arguments\n\ncall syntax:\n\n"
      << _p_arg0
      << " <position-independent-args>\n\nposition-independent-args:\n\n";

    for (size_t x{ 0 }, i, c; x < N; x++)
    {
      _output << _args[x].name;
      for (i = 0, c = 35 - _args[x].name.size(); i < c; i++)
        _output << '.';
      _output << _args[x].description << '\n';
    }
  }
};