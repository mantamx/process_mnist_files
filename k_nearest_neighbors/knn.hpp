// knn.hpp
// (c) Reza Manoochehrian JUN-2024
// info@mantam.com

#include "../LIB_MNIST_IDX_UBYTE_2/mnist_dataset.hpp"
#include <iostream>

class KNearestNeighbors
{
  std::vector<std::valarray<double>> _training_data;
  std::vector<unsigned char> _training_labels;

  enum { STATUS, EXPECTED, RESULT, COUNT, DISTANCE, DURATION };
  static constexpr const char* const P_STR_HEADER{ "Test|Status|Expected|Result|Count|Distance|Duration\n" };
  static constexpr const char* const P_STR_ARRAY_2
    [7] // TOKEN
    [2] // MODE: pretty, delimited
    =
  {
    { ", status = ", "|" }
    , { ", expected = ", "|" }
    , { ", result = ", "|" }
    , { ", count = ", "|" }
    , { ", distance = { ", "|" }
    , { ", time = ", "|"}
  };

public:

  enum class DistanceType { euclidean /*, manhattan */ };
  enum class OutputMode { pretty, delimited };

  KNearestNeighbors() = delete;
  KNearestNeighbors(MnistDatasetBase&&) noexcept;
  ~KNearestNeighbors() = default;
  KNearestNeighbors(const KNearestNeighbors&) = delete;
  KNearestNeighbors& operator=(const KNearestNeighbors&) = delete;
  KNearestNeighbors(KNearestNeighbors&&) = default;
  KNearestNeighbors& operator=(KNearestNeighbors&&) = default;

  void runTest(MnistDatasetBase&&, int, int, int, std::ostream& = std::cout, OutputMode = OutputMode::pretty) noexcept;
};
