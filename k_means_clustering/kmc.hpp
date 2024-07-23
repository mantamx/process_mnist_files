// kmc.hpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#pragma once

#include "../mnist_idx_ubyte/mnist_dataset.hpp"
#include <iostream>

class KMeansClustering
{
  std::vector<std::vector<double>> _training_data;
  std::vector<unsigned char> _training_labels; // for setting clusters via determining most frequent class
  std::vector<std::vector<double>> _cluster_centers;
  size_t _count_clusters;
  size_t _count_training_data;
  size_t _count_features;
  double _training_exit_on_percentage_change;
  int _training_exit_on_unchanged_error;
  int _training_exit_on_iterations;

  std::vector<size_t> _point_to_cluster;
  std::vector<unsigned char> _cluster_class;
  std::vector<unsigned short> _features_mask;

  std::pair<size_t, double> determine_cluster(std::vector<double>&);
  void set_clusters();

public:

  KMeansClustering() = delete;
  KMeansClustering(MnistDatasetBase&&, size_t, double, int, int, size_t, std::ostream & = std::cout) noexcept;
  ~KMeansClustering() = default;
  KMeansClustering(const KMeansClustering&) = delete;
  KMeansClustering& operator=(const KMeansClustering&) = delete;
  KMeansClustering(KMeansClustering&&) = default;
  KMeansClustering& operator=(KMeansClustering&&) = default;

  void runTest(MnistDatasetBase&&, size_t, std::ostream & = std::cout) noexcept;
};
