// knn.cpp
// (c) Reza Manoochehrian JUN-2024
// www.mantam.com

#include "knn.hpp"
#include <map>
#include <chrono>
#include <algorithm>

KNearestNeighbors::KNearestNeighbors(MnistDatasetBase&& training_dataset) noexcept
  : _training_data{ std::move(training_dataset).scaledFeatureVectors() }
  , _training_labels{ std::move(training_dataset).labels() }
  , _count_features{ training_dataset.featureVectorSize() }
{ }

void KNearestNeighbors::runTest
(
  MnistDatasetBase&& test_dataset
  , size_t k
  , size_t count_train_items
  , size_t count_items_to_test
  , std::ostream& output
) noexcept
{
  auto start = std::chrono::high_resolution_clock::now();

  //
  // for keeping track of k-nearest, std::vector<double> performs better (tested on Windows) than std::set<double>
  //

  output << "KNearestNeighbors::runTest(): in\n";

  if (k < 1)
    output << "KNearestNeighbors::runTest(): setting k argument to 1\n";

  std::vector<std::vector<double>> test_data{ std::move(test_dataset).scaledFeatureVectors() };
  std::vector<unsigned char> test_labels{ std::move(test_dataset).labels() };
  const size_t COUNT_K{ k > 0 && k <= _training_data.size() ? k : 3 };
  const size_t COUNT_K_LESS_1{ COUNT_K - 1 };
  size_t current_k{ COUNT_K };
  size_t current_k_less_1{ COUNT_K_LESS_1 };
  const size_t COUNT_TEST_DATA{ count_items_to_test <= test_dataset.itemCount() ? count_items_to_test : test_dataset.itemCount() };
  const size_t COUNT_TRAINING_DATA{ count_train_items <= _training_data.size() ? count_train_items : _training_data.size() };

  std::vector<double> temp_distance(COUNT_TRAINING_DATA);
  std::vector<double> vector_k_nearest(COUNT_TRAINING_DATA);
  auto kn_iterator{ vector_k_nearest.begin() };

  //
  // keep track of distance comparisions in a frequncy map
  // 'result' of the test is the label (among the ascending labels) with the highest 'frequency' (, i.e. with distance in k nearest distances)
  //
  // 'result' is stored in max_frequency_label
  // 'frequency' is stored in max_frequency_count
  //
  std::map<unsigned char, int> frequency_map; // key: label, value: frequency
  auto map_iterator{ frequency_map.end() };
  unsigned char max_frequency_label{ '\0' };
  int max_frequency_count{ 1 };
  //
  //
  //

  std::chrono::high_resolution_clock::time_point start_item_test;
  std::chrono::high_resolution_clock::time_point end_item_test;
  bool found{ false };
  int count_error{ 0 };
  int count_success{ 0 };

  //
  //
  //

  output
    << "KNearestNeighbors::runTest(): TEST_START:\n\tk=" << COUNT_K
    << "\n\ttraining_data=" << COUNT_TRAINING_DATA
    << "\n\ttest_data=" << COUNT_TEST_DATA << '\n';

  static constexpr const char* const P_STR_OK_NOK[]{ "OK", "NOK" };
  enum { TEST_ID, STATUS, EXPECTED, RESULT, COUNT, DISTANCE, DURATION, COUNT_LABELS };
  output << "Test|Status|Expected|Result|Count|Distance|Duration\n";

  auto start_test = std::chrono::high_resolution_clock::now();

  for (size_t iTestData{ 0 }, iTrainingData, z; iTestData < COUNT_TEST_DATA; iTestData++)
  {
    // STEP ONE: calculate distances from reference item to each item in training data
    // COST: count_test_data x count_training_data x calculate distance

    start_item_test = std::chrono::high_resolution_clock::now();

    for (iTrainingData = 0; iTrainingData < COUNT_TRAINING_DATA; iTrainingData++)
    {
      temp_distance[iTrainingData] = pow((test_data[iTestData][0] - _training_data[iTrainingData][0]), 2);
      for (size_t f{ 1 }; f < _count_features; f++)
        temp_distance[iTrainingData] += pow((test_data[iTestData][f] - _training_data[iTrainingData][f]), 2);

      vector_k_nearest[iTrainingData] = temp_distance[iTrainingData] = sqrt(temp_distance[iTrainingData]);
    }

    // STEP TWO: determine k nearest distances
    // COST: count_test_data x count_training_data x std::set::insert()

    // sort ascending
    std::sort(vector_k_nearest.begin(), vector_k_nearest.end());

    // move duplicates to the end
    // check there really are at least k unique nearest
    kn_iterator = std::unique(vector_k_nearest.begin(), vector_k_nearest.end());
    if (kn_iterator - vector_k_nearest.begin() < static_cast<double>(COUNT_K))
    {
      current_k = kn_iterator - vector_k_nearest.begin();
      current_k_less_1 = current_k - 1;
      std::cout << "KNearestNeighbors::runTest(): TEST_" << (iTestData + 1) << ", using unique k " << (kn_iterator - vector_k_nearest.begin()) << " is less than " << COUNT_K << '\n';
    }

    // STEP THREE: how many training data items per k nearest distance = find most used class = prediction = result
    // COST: count_test_data x count_training_data

    for (iTrainingData = 0; iTrainingData < COUNT_TRAINING_DATA; iTrainingData++)
    {
      for (int k = 0; k < current_k; k++)
      {
        if (temp_distance[iTrainingData] == vector_k_nearest[k])
        {
          found = true;
          map_iterator = frequency_map.find(_training_labels[iTrainingData]);
          if (map_iterator == frequency_map.end())
          {
            frequency_map[_training_labels[iTrainingData]] = 1;
            if (max_frequency_label == '\0')
              max_frequency_label = _training_labels[iTrainingData];
          }
          else
          {
            frequency_map[_training_labels[iTrainingData]] = map_iterator->second + 1;
            if (max_frequency_count < map_iterator->second + 1)
            {
              max_frequency_count = map_iterator->second + 1;
              max_frequency_label = _training_labels[iTrainingData];
            }
          }
          break;
        }
      }
    }

    end_item_test = std::chrono::high_resolution_clock::now();

    output
      << "TEST_" << (iTestData + 1)
      << '|' << P_STR_OK_NOK[ test_labels[iTestData] == max_frequency_label ? (count_success++, 0) : (count_error++, 1) ]
      << '|' << static_cast<int>(test_labels[iTestData])
      << '|' << static_cast<int>(max_frequency_label)
      << '|' << max_frequency_count
      << '|';

    for (z = 0; z < current_k_less_1; z++)
      std::cout << vector_k_nearest[z] << ' ';

    output
      << vector_k_nearest[z]
      << '|' << std::chrono::duration_cast<std::chrono::milliseconds>(end_item_test - start_item_test).count()
      << " ms\n";

    //
    //
    //

    frequency_map.clear();
    max_frequency_label = '\0';
    max_frequency_count = 1;
    found = false;
    current_k = COUNT_K;
    current_k_less_1 = COUNT_K_LESS_1;
  }

  auto end_test = std::chrono::high_resolution_clock::now();

  output
  << "KNearestNeighbors::runTest(): TEST_DONE:\n\tk=" << COUNT_K
  << "\n\ttraining_data=" << COUNT_TRAINING_DATA
  << "\n\ttest_data=" << COUNT_TEST_DATA
  << "\n\terror=" << count_error
  << "\n\tsuccess=" << count_success
  << "\n\tpre_test=" << std::chrono::duration_cast<std::chrono::milliseconds>(start_test - start).count()
  << "\n\ttest=" << std::chrono::duration_cast<std::chrono::milliseconds>(end_test - start_test).count();

  auto end = std::chrono::high_resolution_clock::now();

  output
  << "\n\tpost_test=" << std::chrono::duration_cast<std::chrono::milliseconds>(end - end_test).count()
  << "\nKNearestNeighbors::runTest(): out\n";
}
