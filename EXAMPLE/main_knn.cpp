// main_knn.cpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#include "../print_bits/print_bits.hpp"
#include "../k_nearest_neighbors/knn.hpp"
#include <iomanip>
#include <chrono>
#include <iostream>
#include "arg.hpp"

//
//
//

enum
{
  PATH_TRAIN_IMAGES
  , PATH_TRAIN_LABELS
  , PATH_TEST_IMAGES
  , PATH_TEST_LABELS
  , K
  , TRAIN_COUNT
  , TEST_COUNT

  , MAX_COUNT_ARGS
};

static std::array<ArgNameValue, MAX_COUNT_ARGS> ARG_NAME =
{
  ArgNameValue("path_train_images", "interpreted as string; path to MNIST binary file with training image data")
  , ArgNameValue("path_train_labels", "interpreted as string; path to training file with labels")
  , ArgNameValue("path_test_images", "interpreted as string; path to MNIST binary file with test image data")
  , ArgNameValue("path_test_labels", "interpreted as string; path to test file with labels")
  , ArgNameValue("k", "interpreted as integer; defaults to 3, range: 1...<train_count>")
  , ArgNameValue("train_count", "interpretd as integer; number of records from training data to consider for calculations, defaults to 20000")
  , ArgNameValue("test_count", "interpreted as integer; number of records from test data to test, defaults to 50")
};

//
//
//

int main(int argument_count, char** arguments)
{
  std::cout << "main(): in" << std::endl;

  auto myCheck =
    [](std::array<ArgNameValue, MAX_COUNT_ARGS>& args)
    {
      //
      // check arguments
      //

      if (args[K].value.length() == 0)
      {
        args[K].value = "3";
        std::cout << "argument check: setting \'" << args[K].name << "\' to default 3\n";
      }

      if (args[TRAIN_COUNT].value.length() == 0)
      {
        args[TRAIN_COUNT].value = "10000";
        std::cout << "argument check: setting \'" << args[TRAIN_COUNT].name << "\' to default 10000\n";
      }

      if (args[TEST_COUNT].value.length() == 0)
      {
        args[TEST_COUNT].value = "10";
        std::cout << "argument check: setting \'" << args[TEST_COUNT].name << "\' to default 10\n";
      }
    };

  ArgumentHandler argument_handler
  (
    argument_count
    , arguments
    , std::move(ARG_NAME)
    , myCheck
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
  // if handler data is needed for more than one such call (e.g., later for k-means clustering)
  // then from these options, (a) is disabled here, (b) is close to (c), (c) is better:
  // (a) use copies of handler objects. but currently copy constructor / assignments have been disabled.
  // (b) instantiate multipile handler objects.
  // (c) use the dataset constructor with std::vector<unsigned char>
  //
  // sample code for (c)
  //
  //  KNearestNeighbors my_knn
  //  (
  //    MnistDatasetNormalized
  //    (
  //      mnist_handlers[TRAIN_LABELS].items() // copy of data
  //      , mnist_handlers[TRAIN_IMAGES].items() // copy of data
  //      , mnist_handlers[TRAIN_IMAGES].headerField(MnistHandler::HeaderField::row_count) * mnist_handlers[TRAIN_IMAGES].headerField(MnistHandler::HeaderField::column_count)
  //    )
  //  );
  //
  //
  //

  std::cout << "main(): instantiating knn object (VECTOR) with normalized training data...\n";

  start = std::chrono::high_resolution_clock::now();

  //
  // std::move(): after this call, vector of image data is empty,
  // because MnistDataset* constructors move data out of the objects
  //

  KNearestNeighbors knn
  (
    MnistDatasetNormalized
    (
      std::move(   mnist_handlers[TRAIN_LABELS]   )
      , std::move(   mnist_handlers[TRAIN_IMAGES]   )
    )
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): ...knn object (VECTOR) instantiated ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms]\nmain(): running test (VECTOR) with normalized test data...\n";

  start = std::chrono::high_resolution_clock::now();

  knn.runTest
  (
    MnistDatasetNormalized
    (
      std::move(   mnist_handlers[TEST_LABELS]   )
      , std::move(   mnist_handlers[TEST_IMAGES]   )
    )
    , std::stoi(argument_handler._args[K].value)
    , std::stoi(argument_handler._args[TRAIN_COUNT].value)
    , std::stoi(argument_handler._args[TEST_COUNT].value)
    , std::cout
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): ...test (VECTOR) done, KNearestNeighbors::runTest() ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms]\n\nmain(): out\n";

  return 0;
}
