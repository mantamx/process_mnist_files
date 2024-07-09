// main_kmc.cpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#include "../k_means_clustering/kmc.hpp"
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
  , TRAIN_COUNT
  , TRAIN_EXIT_PERCENT
  , TRAIN_EXIT_UNCHANGED
  , TRAIN_EXIT_ITERATIONS
  , K
  , TEST_COUNT
  , OUTPUT_MODE

  , MAX_COUNT_ARGS
};

static std::array<ArgNameValue, MAX_COUNT_ARGS> ARG_NAME =
{
  ArgNameValue("path_train_images", "interpreted as string; path to MNIST binary file with training image data")
  , ArgNameValue("path_train_labels", "interpreted as string; path to training file with labels")
  , ArgNameValue("path_test_images", "interpreted as string; path to MNIST binary file with test image data")
  , ArgNameValue("path_test_labels", "interpreted as string; path to test file with labels")
  , ArgNameValue("train_count", "interpretd as integer; number of records from training data to consider for calculations, defaults to 20000")
  , ArgNameValue("train_exit_on_percentage", "interpreted as double; exit training when percentage change falls below this threshold, defaults to 0.002")
  , ArgNameValue("train_exit_on_unchanged", "interpreted as integer; exit training when total distance / error is unchanged on this many consecutive iterations, defaults to 3")
  , ArgNameValue("train_exit_on_max_iterations", "interpred as integer; exit training after this many iterations, defaults to 50")
  , ArgNameValue("k", "interpred as integer; number of clusters, defaults to 10")
  , ArgNameValue("test_count", "interpreted as integer; number of records from test data to test, defaults to 50")
  , ArgNameValue("output_mode", "interpreted as string; defaults to \"delimited\", range: \"delimited\", \"pretty\"")
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

      if (args[TRAIN_COUNT].value.length() == 0)
      {
        args[TRAIN_COUNT].value = "20000";
        std::cout << "argument check: setting \'" << args[TRAIN_COUNT].name << "\' to default 20000\n";
      }

      if (args[TRAIN_EXIT_PERCENT].value.length() == 0)
      {
        args[TRAIN_EXIT_PERCENT].value = "0.002";
        std::cout << "argument check: setting \'" << args[TRAIN_EXIT_PERCENT].name << "\' to default 0.002\n";
      }

      if (args[TRAIN_EXIT_UNCHANGED].value.length() == 0)
      {
        args[TRAIN_EXIT_UNCHANGED].value = "3";
        std::cout << "argument check: setting \'" << args[TRAIN_EXIT_UNCHANGED].name << "\' to default 3\n";
      }

      if (args[TRAIN_EXIT_ITERATIONS].value.length() == 0)
      {
        args[TRAIN_EXIT_ITERATIONS].value = "50";
        std::cout << "argument check: setting \'" << args[TRAIN_EXIT_ITERATIONS].name << "\' to default 50\n";
      }

      if (args[K].value.length() == 0)
      {
        args[K].value = "10";
        std::cout << "argument check: setting \'" << args[K].name << "\' to default 10\n";
      }

      if (args[TEST_COUNT].value.length() == 0)
      {
        args[TEST_COUNT].value = "50";
        std::cout << "argument check: setting \'" << args[TEST_COUNT].name << "\' to default 50\n";
      }

      if (args[OUTPUT_MODE].value.length() == 0)
      {
        args[OUTPUT_MODE].value = "delimited";
        std::cout << "argument check: setting \'" << args[OUTPUT_MODE].name << "\' to default \'delimited\'\n";
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

  std::cout << "main(): instantiating kmc object with normalized training data\n";

  start = std::chrono::high_resolution_clock::now();

  KMeansClustering kmc
  (
    MnistDatasetNormalized
    (
      std::move(mnist_handlers[TRAIN_LABELS])
      , std::move(mnist_handlers[TRAIN_IMAGES])
    )
    , std::stoi(argument_handler._args[TRAIN_COUNT].value)
    , std::stod(argument_handler._args[TRAIN_EXIT_PERCENT].value)
    , std::stoi(argument_handler._args[TRAIN_EXIT_UNCHANGED].value)
    , std::stoi(argument_handler._args[TRAIN_EXIT_ITERATIONS].value)
    , std::stoi(argument_handler._args[K].value)
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): kmc object instantiated ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms]\nmain(): running test with normalized test data\n";

  start = std::chrono::high_resolution_clock::now();

  kmc.runTest
  (
    MnistDatasetNormalized
    (
      std::move(mnist_handlers[TEST_LABELS])
      , std::move(mnist_handlers[TEST_IMAGES])
    )
    , std::stoi(argument_handler._args[TEST_COUNT].value)
    , std::cout
    , argument_handler._args[OUTPUT_MODE].value == std::string("pretty") ? KMeansClustering::OutputMode::pretty : KMeansClustering::OutputMode::delimited
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): test done, KMeansClustering::runTest() ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms]\n\nmain(): out\n";

  return 0;
}
