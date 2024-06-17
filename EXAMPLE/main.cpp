// main.cpp
// (c) Reza Manoochehrian JUN-2024
// info@mantam.com

#include "../print_bits/print_bits.hpp"
#include "../k_nearest_neighbors/knn.hpp"
#include <iomanip>
#include <chrono>
#include <iostream>

static std::ostream& operator<<(std::ostream& o, const std::valarray<double>& va)
{
  for (const auto x : va)
    o << x << std::endl;
  return o;
}

struct ArgNameValue
{
  ArgNameValue(std::string n) : name{ n } {}
  std::string name;
  std::string value;
} ARG_NAME[] =
{
  ArgNameValue("path_train_images")
  , ArgNameValue("path_train_labels")
  , ArgNameValue("path_test_images")
  , ArgNameValue("path_test_labels")
  , ArgNameValue("k")
  , ArgNameValue("count_train")
  , ArgNameValue("count_test")
  , ArgNameValue("output_mode")
};
enum
{
  PATH_TRAIN_IMAGES
  , PATH_TRAIN_LABELS
  , PATH_TEST_IMAGES
  , PATH_TEST_LABELS
  , K
  , COUNT_TRAIN
  , COUNT_TEST
  , OUTPUT_MODE

  , MAX_COUNT_ARGS
};
const std::string CALL_SYNTAX_PART_1{ "main(): missing arguments\n\ncall syntax: " };
const std::string CALL_SYNTAX_PART_2
{
" <position-independent-args>\n\
\n\
position-independent-args name=value, all names required:\n\
\n\
  -path_train_images=<string>  path to MNIST binary file with training image data\n\
  -path_train_labels=<string>  path to training file with labels\n\
  -path_test_images=<string>   path to test file with images\n\
  -path_test_images=<string>   path to test file with labels\n\
  -k=<integer>                 specific to K Nearest Neighbors, defaults to 3, range: 1...<-count_train>\n\
  -count_train=<integer>       number of records from training data to consider for calculations, defaults to 10000\n\
  -count_test=<integer>        number of records from test data to test, defaults to 10\n\
  -output_mode=<string>        defaults to \"delimited\", range: \"delimited\", \"pretty\"\n\
\n\
\n\
"
};

int main(int argument_count, char** arguments)
{
  if (argument_count - 1 < MAX_COUNT_ARGS)
  {
    std::cout << CALL_SYNTAX_PART_1 << arguments[0] << CALL_SYNTAX_PART_2;
    return 0;
  }

  for (int i{ 1 }; i < argument_count; i++)
  {
    std::string s(arguments[i]);
    std::string::size_type pos{ s.find("=") };
    if (pos != std::string::npos)
    {
      for (int n{ 0 }; n < MAX_COUNT_ARGS; n++)
      {
        if (s.substr(0, pos) == ARG_NAME[n].name)
        {
          ARG_NAME[n].value = s.substr(++pos);
          break;
        }
      }
    }
  }

  // check arguments: all required

  for (int n{ 0 }; n < MAX_COUNT_ARGS; n++)
  {
    if (ARG_NAME[n].value.length() == 0)
    {
      std::cout << CALL_SYNTAX_PART_1 << arguments[0] << CALL_SYNTAX_PART_2;
      return 0;
    }
  }

  std::cout << "main(): in" << std::endl;

  const int ARG_K{ std::stoi(ARG_NAME[K].value) == 0 ? 3 : std::stoi(ARG_NAME[K].value) };
  const int ARG_COUNT_TRAIN{ std::stoi(ARG_NAME[COUNT_TRAIN].value) == 0 ? 10000 : std::stoi(ARG_NAME[COUNT_TRAIN].value) };
  const int ARG_COUNT_TEST{ std::stoi(ARG_NAME[COUNT_TEST].value) == 0 ? 10 : std::stoi(ARG_NAME[COUNT_TEST].value) };
  KNearestNeighbors::OutputMode ARG_OUTPUT_MODE
  {
    ARG_NAME[OUTPUT_MODE].value == std::string("pretty")
    ? KNearestNeighbors::OutputMode::pretty
    : KNearestNeighbors::OutputMode::delimited
  };

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
    MnistHandler(MnistHandler::FileType::image, ARG_NAME[PATH_TRAIN_IMAGES].value)
    , MnistHandler(MnistHandler::FileType::label, ARG_NAME[PATH_TRAIN_LABELS].value)
    , MnistHandler(MnistHandler::FileType::image, ARG_NAME[PATH_TEST_IMAGES].value)
    , MnistHandler(MnistHandler::FileType::label, ARG_NAME[PATH_TEST_LABELS].value)
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

  std::cout << "main(): instantiating knn object with normalized training data...\n";

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
  << "main(): ...knn object instantiated ["
  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  << " ms]\nmain(): running test with normalized test data...\n";

  start = std::chrono::high_resolution_clock::now();

  knn.runTest
  (
    MnistDatasetNormalized
    (
      std::move(   mnist_handlers[TEST_LABELS]   )
      , std::move(   mnist_handlers[TEST_IMAGES]   )
    )
    , ARG_K
    , ARG_COUNT_TRAIN
    , ARG_COUNT_TEST
    , std::cout
    , ARG_OUTPUT_MODE
  );

  end = std::chrono::high_resolution_clock::now();

  std::cout
  << "main(): ...test done, KNearestNeighbors::runTest() ["
  << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()
  << " ms]\n\nmain(): out\n";

  return 0;
}
