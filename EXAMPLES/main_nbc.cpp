// main_kmc.cpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#include "../naive_bayes_classifier/nbc.hpp"
#include <iomanip>
#include <chrono>
#include <iostream>
#include "arg.hpp"

//
//
//

static void standardize(std::vector<double>& v)
{
  const size_t SIZE{ v.size() };

  if (!SIZE)
    return;

  double mean{ 0 };
  for (auto& x : v)
    mean += x;
  mean /= SIZE;

  double stddev{ 0 };
  for (auto& x : v)
    stddev += std::pow((mean - x), 2);
  stddev /= SIZE;
  stddev = std::sqrt(stddev);

  for (size_t x{ 0 }; x < SIZE; x++)
    v[x] = (v[x] - mean) / stddev;

  std::cout << "standardize(): vector size: " << SIZE << '\n';
  std::cout << "standardize(): vector mean: " << mean << '\n';
  std::cout << "standardize(): vector stddev: " << stddev << '\n';
}

enum
{
  PATH_TRAIN_IMAGES
  , PATH_TRAIN_LABELS
  , PATH_TEST_IMAGES
  , PATH_TEST_LABELS
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
  , ArgNameValue("train_count", "interpretd as integer; number of records from training data to consider for calculations, defaults to 20000")
  , ArgNameValue("test_count", "interpreted as integer; number of records from test data to test, defaults to 50")
};

static std::ostream& operator<<(std::ostream& o, const std::vector<double>& v)
{
  for (const auto x : v)
    o << x << '\n';
  return o;
}
static void print_2vd(std::ostream& o, const std::vector<double>& v, const std::vector<double>& v2)
{
  if (v.size() == v2.size())
    for (size_t i{ 0 }, count{ v.size() }; i < count; i++)
      o << static_cast<double>(v[i]) << '|' << v2[i] << '\n';
}

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

      if (args[TEST_COUNT].value.length() == 0)
      {
        args[TEST_COUNT].value = "50";
        std::cout << "argument check: setting \'" << args[TEST_COUNT].name << "\' to default 50\n";
      }
    };

  ArgumentHandler argument_handler
  (
    argument_count
    , arguments
    , std::move(ARG_NAME)
    , myCheck
  );

  auto main_start = std::chrono::high_resolution_clock::now();

  for (size_t x{ 0 }, count{ argument_handler._args.size() }; x < count; x++)
  {
    std::cout << "main(): arg_" << x << ": " << argument_handler._args[x].name << '|' << argument_handler._args[x].value << '\n';
  }

  std::cout << std::fixed << std::setprecision(16);

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


  auto v
  {
    MnistDatasetNormalized
    (
      std::move(mnist_handlers[TRAIN_LABELS])
      , std::move(mnist_handlers[TRAIN_IMAGES])
    ).scaledFeatureVector(101)
  };

  auto v_original{ v };

  standardize(v);

  for (size_t x{ 0 }, count{ v.size() }; x < count; x++)
    std::cout << v_original[x] << '\t' << v[x] << '\n';

  std::cout << "main(): v_MnistDatasetStandardized:" << std::endl;

  auto v_MnistDatasetStandardized
  {
    MnistDatasetStandardized
    (
      MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_LABELS].value)
      , MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value)
    ).scaledFeatureVector(101)
  };

  std::cout << "main(): v_MnistDatasetStandardized.size(): " << v_MnistDatasetStandardized.size() << std::endl;

  auto v_unsigned_char
  {
    MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value).items()
  };

  std::cout << "main(): v_unsigned_char.size(): " << v_unsigned_char.size() << std::endl;

  std::vector<double> v_data_double( 784 );
  std::cout << "main(): v_data_double.size(): " << v_data_double.size() << std::endl;
  for (size_t i{ 0 }; i < 784; i++)
    v_data_double[i] = static_cast<double>(v_unsigned_char[101 * 784 + i]);

  for (size_t x{ 0 }, count{ v_MnistDatasetStandardized.size() }; x < count; x++)
    std::cout << v_data_double[x] << '\t' << v_MnistDatasetStandardized[x] << '\n';


  std::cout << "main(): out\n";

  return 0;

















/*
  print_2vd
  (
    std::cout
    , MnistDatasetBase
    (
      MnistDatasetBase::Scaling::none
      , MnistHandler(MnistHandler::FileType::label, argument_handler._args[PATH_TRAIN_LABELS].value)
      , MnistHandler(MnistHandler::FileType::image, argument_handler._args[PATH_TRAIN_IMAGES].value)
    ).scaledFeatureVector(100)
    , MnistDatasetNormalized
      (
        std::move(mnist_handlers[TRAIN_LABELS])
        , std::move(mnist_handlers[TRAIN_IMAGES])
      ).scaledFeatureVector(100)
  );

  std::cout << "main(): out\n";

  return 0;
*/








  std::cout << "main(): instantiating nbc object with normalized training data\n";

  start = std::chrono::high_resolution_clock::now();

  NaiveBayesClassifier nbc
  (
    MnistDatasetNormalized
    (
      std::move(mnist_handlers[TRAIN_LABELS])
      , std::move(mnist_handlers[TRAIN_IMAGES])
    )
    , std::stoi(argument_handler._args[TRAIN_COUNT].value)
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): nbc object instantiated ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms]\nmain(): running test with normalized test data\n";




  std::cout << "main(): XXXXXXXXXXXXXXXXX\n";
  std::cout << "main(): out\n";

  return 0;






  start = std::chrono::high_resolution_clock::now();

  nbc.runTest
  (
    MnistDatasetNormalized
    (
      std::move(mnist_handlers[TEST_LABELS])
      , std::move(mnist_handlers[TEST_IMAGES])
    )
    , std::stoi(argument_handler._args[TEST_COUNT].value)
    , std::cout
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): test done, NaiveBayesClassifier::runTest() ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
    << " ms]\n";

  auto main_end = std::chrono::high_resolution_clock::now();

  std::cout
    << "main(): out ["
    << std::chrono::duration_cast<std::chrono::milliseconds>(main_end - main_start).count()
    << " ms]\n";

  return 0;
}
