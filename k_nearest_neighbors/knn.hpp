// knn.hpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#pragma once

#include "../mnist_idx_ubyte/mnist_dataset.hpp"
#include <iostream>

class KNearestNeighbors
{
  std::vector<std::vector<double>> _training_data;
  std::vector<unsigned char> _training_labels;
  size_t _count_features;

public:

//  enum class DistanceType { euclidean /*, manhattan */ };

  KNearestNeighbors() = delete;
  KNearestNeighbors(MnistDatasetBase&&) noexcept;
  ~KNearestNeighbors() = default;
  KNearestNeighbors(const KNearestNeighbors&) = delete;
  KNearestNeighbors& operator=(const KNearestNeighbors&) = delete;
  KNearestNeighbors(KNearestNeighbors&&) = default;
  KNearestNeighbors& operator=(KNearestNeighbors&&) = default;

  void runTest(MnistDatasetBase&&, size_t, size_t, size_t, std::ostream& = std::cout) noexcept;
};
