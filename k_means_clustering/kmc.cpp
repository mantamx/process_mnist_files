// kmc.cpp
// (c) Reza Manoochehrian JUL-2024
// www.mantam.com

#include "kmc.hpp"
#include <map>
#include <chrono>
#include <algorithm>
#include <limits>
#include <utility>
#include <tuple>

std::pair<size_t, double> KMeansClustering::determine_cluster
(
  std::vector<double>& v_data
)
{
  double min_distance_square{ 0. };

  for (size_t f{ 0 }; f < _count_features; f++)
    if (_features_mask[f])
      min_distance_square += pow((_cluster_centers[0][f] - v_data[f]), 2);

  double distance_square;
  size_t current_cluster{ 0 };

  for (size_t c{ 1 }, f; c < _count_clusters; c++)
  {
    distance_square = _features_mask[0] ? distance_square = pow((_cluster_centers[c][0] - v_data[0]), 2) : 0.;

    for (f = 1; f < _count_features; f++)
      if (_features_mask[f])
        distance_square += pow((_cluster_centers[c][f] - v_data[f]), 2);

    if (distance_square < min_distance_square)
    {
      current_cluster = c;
      min_distance_square = distance_square;
    }
  }

  return std::make_pair(current_cluster, min_distance_square);
}

KMeansClustering::KMeansClustering
(
  MnistDatasetBase&& training_dataset
  , size_t count_training_items
  , double exit_condition_percentage_change
  , int exit_condition_unchanged_total_distance
  , int exit_condition_max_iterations
  , size_t count_clusters
  , std::ostream& output
) noexcept
  : _training_data{ std::move(training_dataset).scaledFeatureVectors() }
  , _training_labels{ std::move(training_dataset).labels() }
  , _cluster_centers(count_clusters)
  , _count_clusters{ count_clusters }
  , _count_training_data{ count_training_items <= _training_data.size() ? count_training_items : _training_data.size() }
  , _count_features{ _training_data.size() ? _training_data[0].size() : 0 }
  , _training_exit_on_percentage_change{ exit_condition_percentage_change }
  , _training_exit_on_unchanged_error{ exit_condition_unchanged_total_distance }
  , _training_exit_on_iterations{ exit_condition_max_iterations }
  , _point_to_cluster( _count_training_data )
  , _cluster_class(count_clusters)
  , _features_mask( _count_features, 1 )
{
  output << "KMeansClustering::KMeansClustering(): in\n#|CLST_ASSIGN|PCT_CHG|ERR|ERR_MIN|CLST_CHG\n";

  auto in = std::chrono::high_resolution_clock::now();

  //
  // feature that is 0 accross all points has potential
  // to optimize calculation in determine_cluster()
  //

  unsigned short optimization{ 0 };
  for (size_t f{ 0 }; f < _count_features; f++)
  {
    _features_mask[f] = 0;
    for (size_t p{ 0 }; p < _count_training_data; p++)
    {
      if (_training_data[p][f])
      {
        _features_mask[f] = 1;
        break;
      }
    }
    optimization += _features_mask[f];
  }
  std::cout << "KMeansClustering::KMeansClustering(): training optimization:" << (_count_features - optimization) << " of " << _count_features << " times " << _count_training_data << " = " << ((_count_features - optimization) * _count_training_data) << " new-center calculations per line of output below\n";

  //
  //
  //

  std::vector<size_t> cluster_to_point_current_sizes(_count_clusters);
  std::vector<std::vector<size_t>> cluster_to_point(_count_clusters);
  for (size_t x{ 0 }; x < _count_clusters; x++)
    cluster_to_point[x].resize(_count_training_data);

  std::pair<size_t, double> currentCluster_and_minDistanceSquare;
  size_t count_current_cluster_changes{ 0 };

  double total_distance{ 0. };
  double min_total_distance{ std::numeric_limits<double>::max() };
  double percentage_change{ 0 };

  int iteration{ -1 };
  short consecutive_unchanged{ 0 };

  std::chrono::high_resolution_clock::time_point start_cluster_assign;
  std::chrono::high_resolution_clock::time_point end_cluster_assign;

  //
  // STEP ONE: select quasi centroids
  //  

  for (size_t i{ 0 }; i < _count_clusters; i++)
  {
    _cluster_centers[i] = _training_data[ ((_count_training_data / _count_clusters) >> 1) + i * _count_training_data / _count_clusters];
  }

  //
  //
  //

  while (++iteration < _training_exit_on_iterations)
  {
    //
    // STEP TWO: assign cluster
    //

    start_cluster_assign = std::chrono::high_resolution_clock::now();

    for (size_t p{ 0 }; p < _count_training_data; p++)
    {
      currentCluster_and_minDistanceSquare = determine_cluster(_training_data[p]);

      if (_point_to_cluster[p] != currentCluster_and_minDistanceSquare.first)
        count_current_cluster_changes++;

      _point_to_cluster[p] = currentCluster_and_minDistanceSquare.first;
      cluster_to_point[ currentCluster_and_minDistanceSquare.first ][ cluster_to_point_current_sizes[currentCluster_and_minDistanceSquare.first]++ ] = p;

      total_distance += currentCluster_and_minDistanceSquare.second;
    }

    end_cluster_assign = std::chrono::high_resolution_clock::now();

    //
    // break condition
    //

    if (total_distance < min_total_distance)
    {
      percentage_change = (min_total_distance - total_distance) / min_total_distance * 100.;

      output
        << iteration
        << '|' << std::chrono::duration_cast<std::chrono::milliseconds>(end_cluster_assign - start_cluster_assign).count()
        << '|' << percentage_change
        << '|' << total_distance
        << '|' << (iteration ? min_total_distance : 0.)
        << '|' << count_current_cluster_changes
        << '\n';

      if ((min_total_distance - total_distance) / min_total_distance * 100. < _training_exit_on_percentage_change)
      {
        output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_PERCENTAGE_CHANGE\n";
        break;
      }

      min_total_distance = total_distance;

      consecutive_unchanged = 0;
    }
    else if (total_distance == min_total_distance)
    {
      output
        << iteration
        << '|' << std::chrono::duration_cast<std::chrono::milliseconds>(end_cluster_assign - start_cluster_assign).count()
        << '|' << percentage_change
        << '|' << total_distance
        << '|' << (iteration ? min_total_distance : 0.)
        << '|' << count_current_cluster_changes
        << '\n';

      if (++consecutive_unchanged == _training_exit_on_unchanged_error)
      {
        output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_CONSECUTIVE_UNCHANGED\n";
        break;
      }
    }
    else // this can / should never occur
    {
      output
        << iteration
        << '|' << std::chrono::duration_cast<std::chrono::milliseconds>(end_cluster_assign - start_cluster_assign).count()
        << '|' << percentage_change
        << '|' << total_distance
        << '|' << (iteration ? min_total_distance : 0.)
        << '|' << count_current_cluster_changes
        << '\n';

      output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_MIN_TOTAL_DISTANCE_INCREASED\n";
      break;
    }

    //
    // STEP THREE: new (real) centroids
    //
    // new center of cluster: each element of feature vector is the mean of that feature in current cluster points
    //

    double dcount;
    for (size_t xcount, c{ 0 }; c < _count_clusters; c++)
    {
      for (size_t f{ 0 }; f < _count_features; f++)
        _cluster_centers[c][f] = 0.;

      xcount = cluster_to_point_current_sizes[c];

      for (size_t x{ 0 }; x < xcount; x++)
        for (size_t f{ 0 }; f < _count_features; f++)
          _cluster_centers[c][f] += _training_data[ cluster_to_point[c][x] ][f];

      dcount = static_cast<double>(xcount);
      for (size_t f{ 0 }; f < _count_features; f++)
        _cluster_centers[c][f] /= dcount;
    }

    //
    //
    //

    count_current_cluster_changes = 0;
    cluster_to_point_current_sizes.assign(_count_clusters, 0);
    total_distance = percentage_change = 0.;
  }

  if (iteration == _training_exit_on_iterations)
  {
    output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_MAX_ITERATIONS\n";
  }

  //
  //
  //

  set_clusters();

//  for (size_t i{ 0 }; i < _count_clusters; i++)
//    output << "KMeansClustering::KMeansClustering(): cluster_" << i << ": " << static_cast<int>(_cluster_class[i]) << '\n';

  auto out = std::chrono::high_resolution_clock::now();

  output << "KMeansClustering::KMeansClustering(): out " << std::chrono::duration_cast<std::chrono::milliseconds>(out - in).count() << " ms\n";
}

void KMeansClustering::set_clusters()
{
  //
  // WORK-IN-PROGRESS to find a solution to 'collisions' and 'most-frequent-by-narrow-margin'
  //
  // the current insufficient accuracy in assigning clusters reflects in subsequent tests in KMeansClustering::runTest()

  std::vector<std::map<unsigned char, size_t>> v_cluster_frequency_map(_count_clusters);
  std::map<unsigned char, size_t>::iterator map_iterator;
  unsigned char max_frequency_label{ '\0' };

  for (size_t p{ 0 }; p < _count_training_data; p++)
  {
    if ((map_iterator = v_cluster_frequency_map[ _point_to_cluster[p] ].find(_training_labels[p])) == v_cluster_frequency_map[ _point_to_cluster[p] ].end())
    {
      v_cluster_frequency_map[ _point_to_cluster[p] ][ _training_labels[p] ] = 1;
    }
    else
    {
      map_iterator->second++;
    }
  }

  std::vector<std::vector<std::pair<unsigned char, size_t>>> vv(_count_clusters);
  for (size_t c{ 0 }; c < _count_clusters; c++)
  {
    vv[c] = std::vector<std::pair<unsigned char, size_t>>(v_cluster_frequency_map[c].begin(), v_cluster_frequency_map[c].end());
    std::sort
    (
      vv[c].begin()
      , vv[c].end()
      , [](std::pair<unsigned char, size_t>& a, std::pair<unsigned char, size_t>& b) { return a.second > b.second; }
    );
  }

//  std::cout << "CLUSTER|CLASS|COUNT\n";
//  for (size_t c{ 0 }; c < _count_clusters; c++)
//  {
//    for (const auto& x : vv[c])
//      std::cout << c << '|' << static_cast<int>(x.first) << '|' << x.second << '\n';
//
//_cluster_class[c] = vv[c].begin()->first;
//  }

}

void KMeansClustering::runTest
(
  MnistDatasetBase&& test_dataset
  , size_t count_items_to_test
  , std::ostream& output
  , OutputMode output_mode
) noexcept
{
  output << "KMeansClustering::runTest(): in, clusters:" << _count_clusters << '\n';

  std::vector<std::vector<double>> test_data{ std::move(test_dataset).scaledFeatureVectors() };
  std::vector<unsigned char> test_labels{ std::move(test_dataset).labels() };
  const size_t COUNT_TEST_DATA{ count_items_to_test <= test_dataset.itemCount() ? count_items_to_test : test_dataset.itemCount() };
  const int OUTPUT_MODE{ output_mode == OutputMode::pretty ? 0 : 1 };

  bool found{ false };
  int count_error{ 0 };
  int count_success{ 0 };
  static constexpr const char* const P_STR_OK_NOK[]{ "OK", "NOK" };

  //
  //
  //

  output << "KMeansClustering::runTest(): TEST_START:\n\ttest_data=" << COUNT_TEST_DATA << '\n';

  if (OUTPUT_MODE)
    output << KMeansClustering::P_STR_HEADER;

  double distance{ 0. };
  std::pair<size_t, double> currentCluster_and_minDistanceSquare;

  auto start_test = std::chrono::high_resolution_clock::now();

  for (size_t iTestData{ 0 }; iTestData < COUNT_TEST_DATA; iTestData++)
  {
    currentCluster_and_minDistanceSquare = determine_cluster(test_data[iTestData]);

    output

      << "TEST_" << (iTestData + 1)

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::STATUS][OUTPUT_MODE]
      << P_STR_OK_NOK[ test_labels[iTestData] == _cluster_class[currentCluster_and_minDistanceSquare.first] ? (count_success++, 0) : (count_error++, 1) ]

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::EXPECTED][OUTPUT_MODE]
      << static_cast<int>(test_labels[iTestData])

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::RESULT][OUTPUT_MODE]
      << static_cast<int>(_cluster_class[ currentCluster_and_minDistanceSquare.first ])

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::DISTANCE][OUTPUT_MODE]
      << std::sqrt(currentCluster_and_minDistanceSquare.second)
      << '\n';

    //
    //
    //

    found = false;
  }

  auto end_test = std::chrono::high_resolution_clock::now();

  output
    << "KMeansClustering::runTest(): TEST_DONE:\n\ttest_data=" << COUNT_TEST_DATA
    << "\n\terror=" << count_error
    << "\n\tsuccess=" << count_success
    << "\n\ttest=" << std::chrono::duration_cast<std::chrono::milliseconds>(end_test - start_test).count();

  output << "\nKMeansClustering::runTest(): out\n";
}
