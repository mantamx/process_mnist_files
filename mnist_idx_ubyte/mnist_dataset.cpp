// mnist_dataset.cpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#include "mnist_dataset.hpp"
#include <iostream>

MnistDatasetBase::MnistDatasetBase(MnistDatasetBase::Scaling scaling , MnistHandler&& mnistHandlerLabels , MnistHandler&& mnistHandlerImages) noexcept
  : MnistDatasetBase
    (
      scaling
      , std::move(mnistHandlerLabels).items()
      , std::move(mnistHandlerImages).items()
      , static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count))* static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count))
    )
{ }

MnistDatasetBase::MnistDatasetBase(MnistDatasetBase::Scaling scaling , std::vector<unsigned char>&& labels , std::vector<unsigned char>&& data , size_t featureVectorSize) noexcept
  : scaling{ scaling }
  , _itemCount{ labels.size() }
  , _featureVectorSize{ featureVectorSize }
  , _items{ _itemCount }
  , _labels{ std::move(labels) }
{
  std::vector<unsigned char> v{ std::move(data) };

  for (size_t k{ 0 }; k < _itemCount; k++)
  {
    _items[k].resize(_featureVectorSize);
    for (size_t i{ 0 }; i < _featureVectorSize; i++)
    {
      _items[k][i] = v[k * _featureVectorSize + i];
    }
  }
}

size_t MnistDatasetBase::itemCount() const noexcept { return _itemCount; }

size_t MnistDatasetBase::featureVectorSize() const noexcept { return _featureVectorSize; }

unsigned char MnistDatasetBase::label(int index) const { return _labels[index]; }

std::vector<unsigned char> MnistDatasetBase::labels() const& noexcept
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<unsigned char>(_labels.begin(), _labels.end());
}

std::vector<unsigned char> MnistDatasetBase::labels() && noexcept
{
  return std::move(   _labels   );
}

std::vector<std::vector<double>> MnistDatasetBase::scaledFeatureVectors() const& noexcept
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<std::vector<double>>(_items.begin(), _items.end());
}

std::vector<std::vector<double>> MnistDatasetBase::scaledFeatureVectors() && noexcept
{
  return std::move(   _items   );
}

std::vector<double> MnistDatasetBase::scaledFeatureVector(size_t itemIndex) const
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<double>(_items[itemIndex]);
}

double MnistDatasetBase::scaledFeature(size_t itemIndex, size_t featureIndex) const
{
  return _items[itemIndex][featureIndex];
}

/////////////////////////////////////////////////////

MnistDatasetNormalized::MnistDatasetNormalized(MnistHandler&& mnistHandlerLabels, MnistHandler&& mnistHandlerImages) noexcept
  : MnistDatasetNormalized
    (
      std::move(mnistHandlerLabels).items()
      , std::move(mnistHandlerImages).items()
      , static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count))* static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count))
    )
{ }

MnistDatasetNormalized::MnistDatasetNormalized(std::vector<unsigned char>&& labels , std::vector<unsigned char>&& data , size_t featureVectorSize) noexcept
  : MnistDatasetBase(MnistDatasetBase::Scaling::normalization, std::move(labels), std::move(data), featureVectorSize)
  , _featureVectorMin{ }
  , _featureVectorMax{ }
{
  _featureVectorMin.assign(std::begin(_items[0]), std::end(_items[0]));
  _featureVectorMax = _featureVectorMin;

  for (size_t i{ 1 }; i < _itemCount; i++)
  {
    for (size_t k{ 0 }; k < _featureVectorSize; k++)
    {
      if (_items[i][k] < _featureVectorMin[k])
        _featureVectorMin[k] = _items[i][k];
      if (_items[i][k] > _featureVectorMax[k])
        _featureVectorMax[k] = _items[i][k];
    }
  }

  //
  //
  //

  for (size_t i{ 0 }; i < _itemCount; i++)
  {
    for (size_t k{ 0 }; k < _featureVectorSize; k++)
    {
      _items[i][k] = (_featureVectorMax[k] - _featureVectorMin[k]) == 0
        ? 0.
        : (_items[i][k] - _featureVectorMin[k]) / (_featureVectorMax[k] - _featureVectorMin[k]);
    }
  }
}

///////////////////////////////////////////////////

MnistDatasetStandardized::MnistDatasetStandardized(MnistHandler&& mnistHandlerLabels, MnistHandler&& mnistHandlerImages) noexcept
  : MnistDatasetStandardized
  (
    std::move(mnistHandlerLabels).items()
    , std::move(mnistHandlerImages).items()
    , static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count))* static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count))
  )
{ }

MnistDatasetStandardized::MnistDatasetStandardized(std::vector<unsigned char>&& labels , std::vector<unsigned char>&& data , size_t featureVectorSize) noexcept
  : MnistDatasetBase(MnistDatasetBase::Scaling::standardization, std::move(labels), std::move(data), featureVectorSize)
  , _featureVectorMean( _featureVectorSize, 0. )
  , _featureVectorStdev( _featureVectorSize, 0. )
{
  for (size_t i{ 0 }, f; i < _itemCount; i++)
    for (f = 0; f < _featureVectorSize; f++)
      _featureVectorMean[f] += _items[i][f];

  double count{ static_cast<double>(_itemCount) };

  for (size_t f{ 0 }; f < _featureVectorSize; f++)
    _featureVectorMean[f] /= count;

  // done: mean of each feature

  //
  //
  //

  for (size_t i{ 0 }, f; i < _itemCount; i++)
    for (f = 0; f < _featureVectorSize; f++)
      _featureVectorStdev[f] += (_items[i][f] == _featureVectorMean[f]) ? 0. : pow(_items[i][f] - _featureVectorMean[f], 2);

  for (size_t f{ 0 }; f < _featureVectorSize; f++)
    if (_featureVectorStdev[f])
      _featureVectorStdev[f] = sqrt(_featureVectorStdev[f] /= count);

  // done: stddev of each feature

  //
  //
  //

  for (size_t i{ 0 }; i < _itemCount; i++)
  {
    for (size_t f{ 0 }; f < _featureVectorSize; f++)
    {
      if (_featureVectorStdev[f])
      {
        _items[i][f] -= _featureVectorMean[f];
        _items[i][f] /= _featureVectorStdev[f];
      }
    }
  }
}
