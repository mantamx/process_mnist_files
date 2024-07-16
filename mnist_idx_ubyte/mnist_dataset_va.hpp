// mnist_dataset.hpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#pragma once

#ifdef _MNIST_DATASET_VALARRAY

#include "mnist_handler.hpp"
#include <valarray>

class MnistDatasetBase_VA
{
public:

  enum class Scaling { none, normalization, standardization, unit_length };

  MnistDatasetBase_VA(Scaling, MnistHandler&&, MnistHandler&&) noexcept;
  MnistDatasetBase_VA(Scaling, std::vector<unsigned char>&&, std::vector<unsigned char>&&, size_t) noexcept;
  MnistDatasetBase_VA() = delete;
  virtual ~MnistDatasetBase_VA() = default;
  MnistDatasetBase_VA(const MnistDatasetBase_VA&) = delete;
  MnistDatasetBase_VA& operator=(const MnistDatasetBase_VA&) = delete;
  MnistDatasetBase_VA(MnistDatasetBase_VA&&) = default;
  MnistDatasetBase_VA& operator=(MnistDatasetBase_VA&&) = default;

  size_t itemCount() const noexcept;
  size_t featureVectorSize() const noexcept;
  unsigned char label(int) const;
  std::vector<unsigned char> labels() const& noexcept;
  std::vector<unsigned char> labels() && noexcept;

  std::vector<std::valarray<double>> scaledFeatureVectors() const& noexcept;
  std::vector<std::valarray<double>> scaledFeatureVectors() && noexcept;
  std::valarray<double> scaledFeatureVector(size_t) const;
  double scaledFeature(size_t, size_t) const;

protected:

  Scaling scaling;
  size_t _itemCount;
  size_t _featureVectorSize;
  std::vector<std::valarray<double> > _items_va;
  std::vector<unsigned char> _labels;
};

class MnistDatasetNormalized_VA : public MnistDatasetBase_VA
{
  std::vector<double> _featureVectorMin;
  std::vector<double> _featureVectorMax;

public:

  MnistDatasetNormalized_VA(MnistHandler&&, MnistHandler&&) noexcept;
  MnistDatasetNormalized_VA(std::vector<unsigned char>&&, std::vector<unsigned char>&&, size_t) noexcept;
  MnistDatasetNormalized_VA() = delete;
  virtual ~MnistDatasetNormalized_VA() = default;
  MnistDatasetNormalized_VA(const MnistDatasetNormalized_VA&) = delete;
  MnistDatasetNormalized_VA& operator=(const MnistDatasetNormalized_VA&) = delete;
  MnistDatasetNormalized_VA(MnistDatasetNormalized_VA&&) = default;
  MnistDatasetNormalized_VA& operator=(MnistDatasetNormalized_VA&&) = default;
};

class MnistDatasetStandardized_VA : public MnistDatasetBase_VA
{
  std::valarray<double> _featureVectorMean_va;
  std::valarray<double> _featureVectorStdev_va;

public:

  MnistDatasetStandardized_VA(MnistHandler&&, MnistHandler&&) noexcept;
  MnistDatasetStandardized_VA(std::vector<unsigned char>&&, std::vector<unsigned char>&&, size_t) noexcept;
  MnistDatasetStandardized_VA() = delete;
  virtual ~MnistDatasetStandardized_VA() = default;
  MnistDatasetStandardized_VA(const MnistDatasetStandardized_VA&) = delete;
  MnistDatasetStandardized_VA& operator=(const MnistDatasetStandardized_VA&) = delete;
  MnistDatasetStandardized_VA(MnistDatasetStandardized_VA&&) = default;
  MnistDatasetStandardized_VA& operator=(MnistDatasetStandardized_VA&&) = default;
};

#endif
