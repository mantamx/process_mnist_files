// mnist_dataset.cpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#include "mnist_dataset_va.hpp"

#ifdef _MNIST_DATASET_VALARRAY

MnistDatasetBase_VA::MnistDatasetBase_VA(MnistDatasetBase_VA::Scaling scaling , MnistHandler&& mnistHandlerLabels , MnistHandler&& mnistHandlerImages) noexcept
  : MnistDatasetBase_VA
    (
      scaling
      , std::move(mnistHandlerLabels).items()
      , std::move(mnistHandlerImages).items()
      , static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count))* static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count))
    )
{ }

MnistDatasetBase_VA::MnistDatasetBase_VA(MnistDatasetBase_VA::Scaling scaling , std::vector<unsigned char>&& labels , std::vector<unsigned char>&& data , size_t featureVectorSize) noexcept
  : scaling{ scaling }
  , _itemCount{ labels.size() }
  , _featureVectorSize{ featureVectorSize }
  , _items_va{ _itemCount }
  , _labels{ std::move(labels) }
{
  std::vector<unsigned char> v{ std::move(data) };
  std::valarray<double> va;
  size_t count{ v.size() };

  va.resize(count);

  for (size_t k{ 0 }; k < count; k++)
    va[k] = v[k];

  for (size_t k{ 0 }; k < _itemCount; k++)
    _items_va[k] = va[std::slice(k * _featureVectorSize, _featureVectorSize, 1)];
}

size_t MnistDatasetBase_VA::itemCount() const noexcept { return _itemCount; }

size_t MnistDatasetBase_VA::featureVectorSize() const noexcept { return _featureVectorSize; }

unsigned char MnistDatasetBase_VA::label(int index) const { return _labels[index]; }

std::vector<unsigned char> MnistDatasetBase_VA::labels() const& noexcept
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<unsigned char>(_labels.begin(), _labels.end());
}

std::vector<unsigned char> MnistDatasetBase_VA::labels() && noexcept
{
  return std::move(   _labels   );
}

std::vector<std::valarray<double>> MnistDatasetBase_VA::scaledFeatureVectors() const& noexcept
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::vector<std::valarray<double>>(_items_va.begin(), _items_va.end());
}

std::vector<std::valarray<double>> MnistDatasetBase_VA::scaledFeatureVectors() && noexcept
{
  return std::move(   _items_va   );
}

std::valarray<double> MnistDatasetBase_VA::scaledFeatureVector(size_t itemIndex) const
{
  // C++17: std::move() not required as RVO is guaranteed (and performs a move) for the below return statement
  return std::valarray<double>(_items_va[itemIndex]);
}

double MnistDatasetBase_VA::scaledFeature(size_t itemIndex, size_t featureIndex) const
{
  return _items_va[itemIndex][featureIndex];
}

/////////////////////////////////////////////////////

MnistDatasetNormalized_VA::MnistDatasetNormalized_VA(MnistHandler&& mnistHandlerLabels, MnistHandler&& mnistHandlerImages) noexcept
  : MnistDatasetNormalized_VA
    (
      std::move(mnistHandlerLabels).items()
      , std::move(mnistHandlerImages).items()
      , static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count))* static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count))
    )
{ }

MnistDatasetNormalized_VA::MnistDatasetNormalized_VA(std::vector<unsigned char>&& labels , std::vector<unsigned char>&& data , size_t featureVectorSize) noexcept
  : MnistDatasetBase_VA(MnistDatasetBase_VA::Scaling::normalization, std::move(labels), std::move(data), featureVectorSize)
  , _featureVectorMin{ }
  , _featureVectorMax{ }
{
  _featureVectorMin.assign(std::begin(_items_va[0]), std::end(_items_va[0]));
  _featureVectorMax = _featureVectorMin;

  for (size_t i{ 1 }; i < _itemCount; i++)
  {
    for (size_t k{ 0 }; k < _featureVectorSize; k++)
    {
      if (_items_va[i][k] < _featureVectorMin[k])
        _featureVectorMin[k] = _items_va[i][k];
      if (_items_va[i][k] > _featureVectorMax[k])
        _featureVectorMax[k] = _items_va[i][k];
    }
  }

  //
  //
  //

  for (size_t i{ 0 }; i < _itemCount; i++)
  {
    for (size_t k{ 0 }; k < _featureVectorSize; k++)
    {
      _items_va[i][k] = (_featureVectorMax[k] - _featureVectorMin[k]) == 0
        ? 0.
        : (_items_va[i][k] - _featureVectorMin[k]) / (_featureVectorMax[k] - _featureVectorMin[k]);
    }
  }
}

///////////////////////////////////////////////////

MnistDatasetStandardized_VA::MnistDatasetStandardized_VA(MnistHandler&& mnistHandlerLabels, MnistHandler&& mnistHandlerImages) noexcept
  : MnistDatasetStandardized_VA
  (
    std::move(mnistHandlerLabels).items()
    , std::move(mnistHandlerImages).items()
    , static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count))* static_cast<size_t>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count))
  )
{ }

MnistDatasetStandardized_VA::MnistDatasetStandardized_VA(std::vector<unsigned char>&& labels , std::vector<unsigned char>&& data , size_t featureVectorSize) noexcept
  : MnistDatasetBase_VA(MnistDatasetBase_VA::Scaling::standardization, std::move(labels), std::move(data), featureVectorSize)
  , _featureVectorMean_va( 0., _featureVectorSize)
  , _featureVectorStdev_va( 0., _featureVectorSize)
{
  for (size_t i{ 0 }; i < _itemCount; i++)
    _featureVectorMean_va += _items_va[i];

  double count{ static_cast<double>(_itemCount) };

  _featureVectorMean_va /= count;

  // done: mean of each feature

  //
  //
  //

  for (size_t i{ 0 }; i < _itemCount; i++)
    _featureVectorStdev_va += std::pow(_items_va[i] - _featureVectorMean_va, 2);

  for (size_t i{ 0 }; i < _featureVectorSize; i++)
    _featureVectorStdev_va[i] = !_featureVectorStdev_va[i] ? 1. : sqrt(_featureVectorStdev_va[i] /= count);

  // done: stdev of each feature

  //
  //
  //

  for (size_t i{ 0 }; i < _itemCount; i++)
  {
    _items_va[i] -= _featureVectorMean_va;
    _items_va[i] /= _featureVectorStdev_va;
  }
}

#endif
