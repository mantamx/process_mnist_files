// mnist_handler.cpp
// (c) Reza Manoochehrian JUN-2024
// reza@mantam.com

#include "mnist_handler.hpp"
#include "../print_bits/print_bits.hpp"

static uint32_t convertBigToLittleEndian(const unsigned char bytes[4])
{
  return static_cast<uint32_t>( (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | (bytes[3]) );
}

static const char * const EXC_MSG[] =
{
  "MnistHandler::MnistHandler(): error opening image file."
  , "MnistHandler::MnistHandler(): error opening label file."
};

MnistHandler::MnistHandler(FileType type, std::string path)
  : _fileType{ type }
  , _filePath{ path }
  , _fileSize{ 0  }
  , _headerSize{ (type == FileType::image ? 4 : 2) * sizeof uint32_t }
  , _itemSize{ 1 }
  , _header{ }
  , _items{ }
{
  std::ifstream file(path, std::ios::binary);
  if (!file.is_open())
  {
    throw std::invalid_argument(EXC_MSG[type == FileType::image ? 0: 1]);
  }

  file.seekg(0, std::ios::end);
  _fileSize = file.tellg();
  file.seekg(0, std::ios::beg);

  unsigned char fourBytes[4]{ 0 };

  file.read(reinterpret_cast<char*>(fourBytes), sizeof fourBytes);
  _header[static_cast<int>(HeaderField::magic_number)] = convertBigToLittleEndian(fourBytes);

  file.read(reinterpret_cast<char*>(fourBytes), sizeof fourBytes);
  _header[static_cast<int>(HeaderField::item_count)] = convertBigToLittleEndian(fourBytes);

  if (_fileType == FileType::image)
  {
    file.read(reinterpret_cast<char*>(fourBytes), sizeof fourBytes);
    _header[static_cast<int>(HeaderField::row_count)] = convertBigToLittleEndian(fourBytes);

    file.read(reinterpret_cast<char*>(fourBytes), sizeof fourBytes);
    _header[static_cast<int>(HeaderField::column_count)] = convertBigToLittleEndian(fourBytes);

    _itemSize = _header[static_cast<int>(HeaderField::row_count)] * _header[static_cast<int>(HeaderField::column_count)];
  }
  else
  {
    _header[2] = _header[3] = 0;
  }

  _items.resize(_fileSize - _headerSize);
  file.read(reinterpret_cast<char*>(&_items[0]), _fileSize - _headerSize);
  file.close();
}

std::vector<unsigned char> MnistHandler::items() const& noexcept
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<unsigned char>(_items.begin(), _items.end());
}

std::vector<unsigned char> MnistHandler::items() && noexcept
{
  return std::move(_items);
}

std::array<uint32_t, 4> MnistHandler::headerFields() const noexcept
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::array<uint32_t, 4>{   _header[0], _header[1], _header[2], _header[3]   };
}

uint32_t MnistHandler::headerField(HeaderField field) const noexcept
{
  return _header[static_cast<int>(field)];
}

unsigned char MnistHandler::readByte(int index) const
{
  return _items[index];
}

std::vector<unsigned char> MnistHandler::readItem(int itemIndex) const
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<unsigned char>(_items.begin() + itemIndex * _itemSize, _items.begin() + (itemIndex + 1) * _itemSize);
}

MnistHandler::FileType MnistHandler::fileType() const noexcept { return _fileType; }

std::string MnistHandler::filePath() const noexcept { return _filePath; }

std::streamsize MnistHandler::fileSize() const noexcept { return _fileSize; }

int MnistHandler::headerSize() const noexcept { return _headerSize; }

int MnistHandler::itemSize() const noexcept { return _itemSize; }

void MnistHandler::prettyPrintItem(FileType type, std::ostream& o, int index) const
{
  if (type == FileType::image)
  {
    print_bits
    (
      PrintMode::bytes_pretty_binary
      , o
      , _items
      , index * _itemSize
      , _itemSize
      , static_cast<unsigned long>(_header[static_cast<int>(HeaderField::row_count)])
    );
  }
  else
  {
    o << static_cast<unsigned int>(_items[index]) << std::endl;
  }
}
