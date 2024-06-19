// mnist_handler.hpp
// (c) Reza Manoochehrian JUN-2024
// reza@mantam.com

#pragma once

#include <vector>
#include <array>
#include <string>

class MnistHandler
{
public:

  enum class FileType { image, label };
  enum class HeaderField { magic_number, item_count, row_count, column_count };

  MnistHandler() = delete;
  MnistHandler(FileType, std::string);
  virtual ~MnistHandler() = default;
  MnistHandler(const MnistHandler&) = delete;
  MnistHandler& operator=(const MnistHandler&) = delete;
  MnistHandler(MnistHandler&&) = default;
  MnistHandler& operator=(MnistHandler&&) = default;

  std::vector<unsigned char> items() const & noexcept;
  std::vector<unsigned char> items() && noexcept;
  std::array<uint32_t, 4> headerFields() const noexcept;

  uint32_t headerField(HeaderField) const noexcept;
  unsigned char readByte(int) const;
  std::vector<unsigned char> readItem(int) const;

  FileType fileType() const noexcept;
  std::string filePath() const noexcept;
  std::streamsize fileSize() const noexcept;
  int headerSize() const noexcept;
  int itemSize() const noexcept;

  void prettyPrintItem(FileType, std::ostream&, int) const;

protected:

  FileType _fileType;
  std::string _filePath;
  std::streamsize _fileSize;
  int _headerSize;
  int _itemSize;
  uint32_t _header[4];
  std::vector<unsigned char> _items;
};
