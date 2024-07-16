// knn.hpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#pragma once

#ifdef _MNIST_DATASET_VALARRAY

#include "../mnist_idx_ubyte/mnist_dataset_va.hpp"
#include <iostream>

class KNearestNeighbors_VA
{
  std::vector<std::valarray<double>> _training_data;
  std::vector<unsigned char> _training_labels;

  enum { STATUS, EXPECTED, RESULT, COUNT, DISTANCE, DURATION };
  static constexpr const char* const P_STR_HEADER{ "Test|Status|Expected|Result|Count|Distance|Duration\n" };
  static constexpr const char* const P_STR_ARRAY_2
    [6] // TOKEN
    [2] // MODE: pretty, delimited
    =
  {
    { ", status = ", "|" }
    , { ", expected = ", "|" }
    , { ", result = ", "|" }
    , { ", count = ", "|" }
    , { ", distance = { ", "|" }
    , { ", duration = ", "|"}
  };

public:

//  enum class DistanceType { euclidean /*, manhattan */ };
  enum class OutputMode { pretty, delimited };

  KNearestNeighbors_VA() = delete;
  KNearestNeighbors_VA(MnistDatasetBase_VA&&) noexcept;
  ~KNearestNeighbors_VA() = default;
  KNearestNeighbors_VA(const KNearestNeighbors_VA&) = delete;
  KNearestNeighbors_VA& operator=(const KNearestNeighbors_VA&) = delete;
  KNearestNeighbors_VA(KNearestNeighbors_VA&&) = default;
  KNearestNeighbors_VA& operator=(KNearestNeighbors_VA&&) = default;

  void runTest(MnistDatasetBase_VA&&, size_t, size_t, size_t, std::ostream& = std::cout, OutputMode = OutputMode::pretty) noexcept;
};

#endif
