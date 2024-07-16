// mnist_dataset.hpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#pragma once

#include "mnist_handler.hpp"
#include <vector>

class MnistDatasetBase
{
public:

  enum class Scaling { none, normalization, standardization, unit_length };

  MnistDatasetBase(Scaling, MnistHandler&&, MnistHandler&&) noexcept;
  MnistDatasetBase(Scaling, std::vector<unsigned char>&&, std::vector<unsigned char>&&, size_t) noexcept;
  MnistDatasetBase() = delete;
  virtual ~MnistDatasetBase() = default;
  MnistDatasetBase(const MnistDatasetBase&) = delete;
  MnistDatasetBase& operator=(const MnistDatasetBase&) = delete;
  MnistDatasetBase(MnistDatasetBase&&) = default;
  MnistDatasetBase& operator=(MnistDatasetBase&&) = default;

  size_t itemCount() const noexcept;
  size_t featureVectorSize() const noexcept;
  unsigned char label(int) const;
  std::vector<unsigned char> labels() const& noexcept;
  std::vector<unsigned char> labels() && noexcept;

  std::vector<std::vector<double>> scaledFeatureVectors() const& noexcept;
  std::vector<std::vector<double>> scaledFeatureVectors() && noexcept;
  std::vector<double> scaledFeatureVector(size_t) const;
  double scaledFeature(size_t, size_t) const;

protected:

  Scaling scaling;
  size_t _itemCount;
  size_t _featureVectorSize;
  std::vector<std::vector<double> > _items;
  std::vector<unsigned char> _labels;
};

class MnistDatasetNormalized : public MnistDatasetBase
{
  std::vector<double> _featureVectorMin;
  std::vector<double> _featureVectorMax;

public:

  MnistDatasetNormalized(MnistHandler&&, MnistHandler&&) noexcept;
  MnistDatasetNormalized(std::vector<unsigned char>&&, std::vector<unsigned char>&&, size_t) noexcept;
  MnistDatasetNormalized() = delete;
  virtual ~MnistDatasetNormalized() = default;
  MnistDatasetNormalized(const MnistDatasetNormalized&) = delete;
  MnistDatasetNormalized& operator=(const MnistDatasetNormalized&) = delete;
  MnistDatasetNormalized(MnistDatasetNormalized&&) = default;
  MnistDatasetNormalized& operator=(MnistDatasetNormalized&&) = default;
};

class MnistDatasetStandardized : public MnistDatasetBase
{
  std::vector<double> _featureVectorMean;
  std::vector<double> _featureVectorStdev;

public:

  MnistDatasetStandardized(MnistHandler&&, MnistHandler&&) noexcept;
  MnistDatasetStandardized(std::vector<unsigned char>&&, std::vector<unsigned char>&&, size_t) noexcept;
  MnistDatasetStandardized() = delete;
  virtual ~MnistDatasetStandardized() = default;
  MnistDatasetStandardized(const MnistDatasetStandardized&) = delete;
  MnistDatasetStandardized& operator=(const MnistDatasetStandardized&) = delete;
  MnistDatasetStandardized(MnistDatasetStandardized&&) = default;
  MnistDatasetStandardized& operator=(MnistDatasetStandardized&&) = default;
};
