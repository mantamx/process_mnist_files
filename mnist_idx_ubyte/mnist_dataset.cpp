// mnist_dataset.cpp
// (c) Reza Manoochehrian JUN-2024
// info@mantam.com

#pragma once

#include "mnist_dataset.hpp"

MnistDatasetBase::MnistDatasetBase
(
  MnistDatasetBase::Scaling scaling
  , MnistHandler&& mnistHandlerImages
  , MnistHandler&& mnistHandlerLabels
) noexcept
  : scaling{ scaling }
  , _itemCount{ static_cast<int>(mnistHandlerImages.headerField(MnistHandler::HeaderField::item_count)) }
  , _featureVectorSize{ static_cast<int>(mnistHandlerImages.headerField(MnistHandler::HeaderField::row_count)) * static_cast<int>(mnistHandlerImages.headerField(MnistHandler::HeaderField::column_count)) }
  , _items{ static_cast<size_t>(_itemCount) }
  , _labels{ std::move(mnistHandlerLabels).items() }
{
  std::vector<unsigned char> vi{ std::move(mnistHandlerImages).items() }; // ...items() moves
  std::valarray<double> va;
  int count{ static_cast<int>(vi.size()) };

  va.resize(count);

  for (int k{ 0 }; k < count; k++)
    va[k] = vi[k];

  for (int k{ 0 }; k < _itemCount; k++)
    _items[k] = va[std::slice(k * _featureVectorSize, _featureVectorSize, 1)];
}

int MnistDatasetBase::itemCount() const noexcept { return _itemCount; }

int MnistDatasetBase::featureVectorSize() const noexcept { return _featureVectorSize; }

unsigned char MnistDatasetBase::label(int index) const { return _labels[index]; }

std::vector<unsigned char> MnistDatasetBase::labels() const& noexcept
{
  return std::move(std::vector<unsigned char>(_labels.begin(), _labels.end()));
}

std::vector<unsigned char> MnistDatasetBase::labels() && noexcept
{
  return std::move(   _labels   );
}

std::vector<std::valarray<double>> MnistDatasetBase::scaledFeatureVectors() const& noexcept
{
  return std::move(   std::vector<std::valarray<double>>(_items.begin(), _items.end())   );
}

std::vector<std::valarray<double>> MnistDatasetBase::scaledFeatureVectors() && noexcept
{
  return std::move(   _items   );
}

std::valarray<double> MnistDatasetBase::scaledFeatureVector(int itemIndex) const
{
  return _items[itemIndex];
}

double MnistDatasetBase::scaledFeature(int itemIndex, int featureIndex) const
{
  return _items[itemIndex][featureIndex];
}

/////////////////////////////////////////////////////

MnistDatasetNormalized::MnistDatasetNormalized(MnistHandler&& mhimages, MnistHandler&& mhlabels) noexcept
  : MnistDatasetBase(MnistDatasetBase::Scaling::normalization, std::forward<MnistHandler>(mhimages), std::forward<MnistHandler>(mhlabels))
  , _featureVectorMin{ }
  , _featureVectorMax{ }
{
  _featureVectorMin.assign(std::begin(_items[0]), std::end(_items[0]));
  _featureVectorMax = _featureVectorMin;

  for (int i{ 1 }; i < _itemCount; i++)
  {
    for (int k{ 0 }; k < _featureVectorSize; k++)
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

  for (int i{ 0 }; i < _itemCount; i++)
  {
    for (int k{ 0 }; k < _featureVectorSize; k++)
    {
      _items[i][k] = (_featureVectorMax[k] - _featureVectorMin[k]) == 0
        ? 0.
        : (_items[i][k] - _featureVectorMin[k]) / (_featureVectorMax[k] - _featureVectorMin[k]);
    }
  }
}

///////////////////////////////////////////////////

MnistDatasetStandardized::MnistDatasetStandardized(MnistHandler&& mhimages, MnistHandler&& mhlabels) noexcept
  : MnistDatasetBase(MnistDatasetBase::Scaling::standardization, std::forward<MnistHandler>(mhimages), std::forward<MnistHandler>(mhlabels))
  , _featureVectorMean( 0., _featureVectorSize)
  , _featureVectorStdev( 0., _featureVectorSize)
{
  for (int i{ 1 }; i < _itemCount; i++)
    _featureVectorMean += _items[i];

  _featureVectorMean /= _itemCount; // done: mean of each feature

  //
  //
  //

  for (int i{ 0 }; i < _itemCount; i++)
    _featureVectorStdev += std::pow(_items[i] - _featureVectorMean, 2);
  _featureVectorStdev = std::sqrt(_featureVectorStdev /= _itemCount); // done: stdev of each feature

  //
  //
  //

  for (int i{ 0 }; i < _itemCount; i++)
  {
    _items[i] -= _featureVectorMean;
    _items[i] /= _featureVectorStdev; // SAFE ? STDEV 0 IS RULED OUT !!!
  }
}
