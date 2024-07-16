// main_knn.cpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#include "../print_bits/print_bits.hpp"
#include "../mnist_idx_ubyte/mnist_handler.hpp"
#include "../mnist_idx_ubyte/mnist_dataset.hpp"
#include <iomanip>
#include <chrono>
#include <iostream>
#include "arg.hpp"

#ifdef _MNIST_DATASET_VALARRAY
#include "../mnist_idx_ubyte/mnist_dataset_va.hpp"
#endif

//
//
//

enum
{
  PATH_TRAIN_IMAGES
  , PATH_TRAIN_LABELS
  , PATH_TEST_IMAGES
  , PATH_TEST_LABELS
  , MAX_COUNT_ARGS
};

static std::array<ArgNameValue, MAX_COUNT_ARGS> ARG_NAME =
{
  ArgNameValue("path_train_images", "interpreted as string; path to MNIST binary file with training image data")
  , ArgNameValue("path_train_labels", "interpreted as string; path to training file with labels")
  , ArgNameValue("path_test_images", "interpreted as string; path to MNIST binary file with test image data")
  , ArgNameValue("path_test_labels", "interpreted as string; path to test file with labels")
};

#ifdef _MNIST_DATASET_VALARRAY

static std::ostream& operator<<(std::ostream& o, const std::vector<std::valarray<double>>& v)
{
  for (const auto& x : v)
    for (const auto f : x)
      o << f << std::endl;
  return o;
}

static std::ostream& operator<<(std::ostream& o, const std::vector<std::vector<double>>& v)
{
  for (const auto& x : v)
    for (const auto f : x)
      o << f << std::endl;
  return o;
}

#endif

//
//
//

int main(int argument_count, char** arguments)
{
  std::cout << "main(): in" << std::endl;

  ArgumentHandler argument_handler
  (
    argument_count
    , arguments
    , std::move(ARG_NAME)
    , [](std::array<ArgNameValue, MAX_COUNT_ARGS>& args)
      {
        std::cout << "check arguments: nothing to do\n";
      }
  );

  for (size_t x{ 0 }, count{ argument_handler._args.size() }; x < count; x++)
  {
    std::cout << "main(): arg_" << x << ": " << argument_handler._args[x].name << '|' << argument_handler._args[x].value << '\n';
  }

  std::cout << std::fixed << std::setprecision(6);

  //
  //
  //

  enum { TRAIN_IMAGES, TRAIN_LABELS, TEST_IMAGES, TEST_LABELS, COUNT_FILES };
  static const char* const P_FILES[] = { "TRAIN_IMAGES", "TRAIN_LABELS", "TEST_IMAGES", "TEST_LABELS" };

  std::cout << "main(): reading 4 files: training images, training labels, test images, test labels...\n";

  auto start = std::chrono::high_resolution_clock::now();

  std::array<MnistHandler, COUNT_FILES>mnist_handlers
  {
    MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value)
    , MnistHandler(MnistHandler::FileType::label, argument_handler._args[PATH_TRAIN_LABELS].value)
    , MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TEST_IMAGES].value)
    , MnistHandler(MnistHandler::FileType::label, argument_handler._args[PATH_TEST_LABELS].value)
  };

  auto end = std::chrono::high_resolution_clock::now();

  std::cout
  << "main(): ...4 files read ["
  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  << " ms]\n\n";

  //
  //
  //

  std::cout << "main(): skip header, print 32 bytes starting with 159th byte, 4 bytes per line, with line numbers\n\n";
  print_bits
  (
    PrintMode::bits
    , std::cout
    , mnist_handlers[TRAIN_IMAGES].items()
    , 158
    , 32
    , 4
    , true
  );
  std::cout << "\nmain(): pretty-print the same: 0-bytes as space, otherwise asterix\n\n";
  print_bits
  (
    PrintMode::bytes_pretty_binary
    , std::cout
    , mnist_handlers[TRAIN_IMAGES].items()
    , 158
    , 32
    , 4
    , true
  );
  std::cout << '\n';

  //
  //
  //

  for (int f{ 0 }; f < COUNT_FILES; f++)
  {
    std::cout << "main(): " << P_FILES[f] << " header values...\n";
    std::cout << "main(): magic number: " << mnist_handlers[f].headerField(MnistHandler::HeaderField::magic_number) << '\n';
    std::cout << "main(): item count: " << mnist_handlers[f].headerField(MnistHandler::HeaderField::item_count) << '\n';
    if (MnistHandler::FileType::image == mnist_handlers[f].fileType())
    {
      std::cout << "main(): row count: " << mnist_handlers[f].headerField(MnistHandler::HeaderField::row_count) << '\n';
      std::cout << "main(): column count: " << mnist_handlers[f].headerField(MnistHandler::HeaderField::column_count) << '\n';
    }
    std::cout << std::endl;
  }

  for (const auto f : { TRAIN_LABELS, TEST_LABELS } )
  {
    std::cout << "main(): " << P_FILES[f] << " labels 0, 11 and 40...\n";
    mnist_handlers[f].prettyPrintItem(MnistHandler::FileType::label, std::cout, 0);
    mnist_handlers[f].prettyPrintItem(MnistHandler::FileType::label, std::cout, 11);
    mnist_handlers[f].prettyPrintItem(MnistHandler::FileType::label, std::cout, 40);
    std::cout << std::endl;
  }

  for (const auto f : { TRAIN_IMAGES, TEST_IMAGES })
  {
    std::cout << "main(): " << P_FILES[f] << " images 0, 11 and 40...\n";
    mnist_handlers[f].prettyPrintItem(MnistHandler::FileType::image, std::cout, 0);
    mnist_handlers[f].prettyPrintItem(MnistHandler::FileType::image, std::cout, 11);
    mnist_handlers[f].prettyPrintItem(MnistHandler::FileType::image, std::cout, 40);
    std::cout << std::endl;
  }

#ifdef _MNIST_DATASET_VALARRAY

  std::cout << "main(): compare handler implementations based on std::valarray and std::vector\n";

  //
  // normalized features
  //

  MnistDatasetNormalized_VA n
  (
    std::move(mnist_handlers[TRAIN_LABELS])
    , std::move(mnist_handlers[TRAIN_IMAGES])
  );

  std::vector<std::valarray<double>> fv_array
  =
  {
    n.scaledFeatureVector(0)
    , n.scaledFeatureVector(1)
    , n.scaledFeatureVector(10)
    , n.scaledFeatureVector(11)
    , n.scaledFeatureVector(110)
    , n.scaledFeatureVector(1000)
  };

  std::cout << "main(): print normalized valarray\n";
  std::cout << fv_array << std::endl;

  MnistDatasetNormalized n_v
  (
    std::move(MnistHandler(MnistHandler::FileType::label, argument_handler._args[PATH_TRAIN_LABELS].value))
    , std::move(MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value))
  );

  std::vector<std::vector<double>> fv_v_array
  =
  {
    n_v.scaledFeatureVector(0)
    , n_v.scaledFeatureVector(1)
    , n_v.scaledFeatureVector(10)
    , n_v.scaledFeatureVector(11)
    , n_v.scaledFeatureVector(110)
    , n_v.scaledFeatureVector(1000)
  };

  std::cout << "main(): print normalized vector\n";
  std::cout << fv_v_array << std::endl;

  //
  // standardized features
  //

  {
    MnistDatasetStandardized_VA n
    (
      MnistHandler(MnistHandler::FileType::label, argument_handler._args[PATH_TRAIN_LABELS].value)
      , MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value)
    );

    std::vector<std::valarray<double>> fv_array
    =
    {
      n.scaledFeatureVector(0)
      , n.scaledFeatureVector(1)
      , n.scaledFeatureVector(10)
      , n.scaledFeatureVector(11)
      , n.scaledFeatureVector(110)
      , n.scaledFeatureVector(1000)
    };

    std::cout << "main(): print standardized valarray\n";
    std::cout << fv_array << std::endl;

    MnistDatasetStandardized n_v
    (
      MnistHandler(MnistHandler::FileType::label, argument_handler._args[PATH_TRAIN_LABELS].value)
      , MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value)
    );

    std::vector<std::vector<double>> fv_v_array
    =
    {
      n_v.scaledFeatureVector(0)
      , n_v.scaledFeatureVector(1)
      , n_v.scaledFeatureVector(10)
      , n_v.scaledFeatureVector(11)
      , n_v.scaledFeatureVector(110)
      , n_v.scaledFeatureVector(1000)
    };

    std::cout << "main(): print stadardized vector\n";
    std::cout << fv_v_array << std::endl;
  }
#endif

  std::cout << "main(): out\n";

  return 0;
}

