// kmc.hpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#pragma once

#include "../mnist_idx_ubyte/mnist_dataset.hpp"
#include <iostream>

class KMeansClustering
{
  std::vector<std::valarray<double>> _training_data;
  std::vector<unsigned char> _training_labels; // for verification
  std::vector<std::valarray<double>> _cluster_centers;
  size_t _count_training_data;
  size_t _count_clusters;
  double _training_exit_on_percentage_change;
  int _training_exit_on_unchanged_error;
  int _training_exit_on_iterations;

  std::vector<size_t> _point_to_cluster;
  std::vector<size_t> _cluster_to_point_current_sizes;
  std::vector<std::vector<size_t>> _cluster_to_point;

  enum { STATUS, EXPECTED, RESULT, DISTANCE, DURATION };
  static constexpr const char* const P_STR_HEADER{ "Test|Status|Expected|Result|Distance\n" };
  static constexpr const char* const P_STR_ARRAY_2
    [4] // TOKEN
    [2] // MODE: pretty, delimited
    =
  {
    { ", status = ", "|" }
    , { ", expected = ", "|" }
    , { ", result = ", "|" }
    , { ", distance = { ", "|" }
  };

public:

  enum class OutputMode { pretty, delimited };

  KMeansClustering() = delete;
  KMeansClustering(MnistDatasetBase&&, size_t, double, int, int, size_t, std::ostream & = std::cout) noexcept;
  ~KMeansClustering() = default;
  KMeansClustering(const KMeansClustering&) = delete;
  KMeansClustering& operator=(const KMeansClustering&) = delete;
  KMeansClustering(KMeansClustering&&) = default;
  KMeansClustering& operator=(KMeansClustering&&) = default;

  void runTest(MnistDatasetBase&&, size_t, std::ostream & = std::cout, OutputMode = OutputMode::pretty) noexcept;
};
