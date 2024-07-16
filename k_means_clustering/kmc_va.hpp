// kmc.hpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#pragma once

#ifdef _MNIST_DATASET_VALARRAY

#include "../mnist_idx_ubyte/mnist_dataset_va.hpp"
#include <iostream>

class KMeansClustering_VA
{
  std::vector<std::valarray<double>> _training_data;
  std::vector<unsigned char> _training_labels; // for setting clusters via determining most frequent class
  std::vector<std::valarray<double>> _cluster_centers;
  size_t _count_training_data;
  size_t _count_clusters;
  double _training_exit_on_percentage_change;
  int _training_exit_on_unchanged_error;
  int _training_exit_on_iterations;

  std::vector<size_t> _point_to_cluster;
  std::vector<unsigned char> _cluster_class;

  void set_clusters();

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

  KMeansClustering_VA() = delete;
  KMeansClustering_VA(MnistDatasetBase_VA&&, size_t, double, int, int, size_t, std::ostream & = std::cout) noexcept;
  ~KMeansClustering_VA() = default;
  KMeansClustering_VA(const KMeansClustering_VA&) = delete;
  KMeansClustering_VA& operator=(const KMeansClustering_VA&) = delete;
  KMeansClustering_VA(KMeansClustering_VA&&) = default;
  KMeansClustering_VA& operator=(KMeansClustering_VA&&) = default;

  void runTest(MnistDatasetBase_VA&&, size_t, std::ostream & = std::cout, OutputMode = OutputMode::pretty) noexcept;
};

#endif
