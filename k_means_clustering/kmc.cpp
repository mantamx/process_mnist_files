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

static std::pair<size_t, double> determine_cluster
(
  std::vector<std::valarray<double>>& vva_cluster
  , std::valarray<double>& va_data
  , size_t count_clusters
)
{
  double distance_square{ std::pow((vva_cluster[0] - va_data), 2).sum() };
  double distance{ std::sqrt(distance_square) };
  double min_distance_square{ distance_square };
  double min_distance{ distance };
  size_t current_cluster{ 0 };

  for (size_t c{ 1 }; c < count_clusters; c++)
  {
    distance_square = std::pow((vva_cluster[c] - va_data), 2).sum();
    distance = std::sqrt(distance_square);

    if (min_distance < distance)
    {
      distance = min_distance;
      distance_square = min_distance_square;
    }
    else
    {
      current_cluster = c;
    }

    min_distance = distance;
    min_distance_square = distance_square;
  }

  return std::make_pair(current_cluster, min_distance_square);
}

static void validate_clusters
(
  std::ostream& output
  , size_t count_clusters
  , std::vector<size_t>& point_to_cluster
  , std::vector<size_t>& cluster_to_point_current_sizes
  , std::vector<std::vector<size_t>>& cluster_to_point
, std::vector<unsigned char>& tlab
)
{
  output << "KMeansClustering::KMeansClustering(): show cluster sizes\n";
  output << "CLST|SIZE\n";
  for (size_t c{ 0 }; c < count_clusters; c++)
  {
    output << c << '|' << cluster_to_point_current_sizes[c] << '\n';
  }

  output << "KMeansClustering::KMeansClustering(): show cluster of first 300 points\n";
  output << "SEQ|IDX\n";
  for (size_t p{ 0 }; p < count_clusters * 30; p++)
  {
    output << p << '|' << point_to_cluster[p] << '\n';
  }

  output << "KMeansClustering::KMeansClustering(): show per cluster 30 points\n";
  output << "CLST|SEQ|IDX\n";
  for (size_t c{ 0 }, s; c < count_clusters; c++)
  {
    for (s = 0; s < 30; s++)
    {
      output << c << '|' << s << '|' << cluster_to_point[c][s] << '\n';
    }
  }

  output << "KMeansClustering::KMeansClustering(): show per cluster 30 labels\n";
  output << "CLST|SEQ|PNT|EXMP\n";
  for (size_t c{ 0 }, s; c < count_clusters; c++)
  {
    for (s = 0; s < 30; s++)
    {
      output << c << '|' << s << '|' << cluster_to_point[c][s] << '|' << static_cast<unsigned int>(tlab[cluster_to_point[c][s]]) << '\n';
    }
  }
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
  , _count_clusters{ count_clusters }
  , _training_exit_on_percentage_change{ exit_condition_percentage_change }
  , _training_exit_on_unchanged_error{ exit_condition_unchanged_total_distance }
  , _training_exit_on_iterations{ exit_condition_max_iterations }
  , _cluster_centers( count_clusters )
  , _count_training_data{ count_training_items <= _training_data.size() ? count_training_items : _training_data.size() }
  , _point_to_cluster( _count_training_data )
  , _cluster_to_point_current_sizes( count_clusters )
  , _cluster_to_point(count_clusters)
{
  output << "KMeansClustering::KMeansClustering(): in\n#|CLST_ASSIGN|PCT_CHG|ERR|ERR_MIN\n";

  auto in = std::chrono::high_resolution_clock::now();

  for (size_t x{ 0 }; x < _count_clusters; x++)
    _cluster_to_point[x].resize(_count_training_data);

  double distance{ 0. };
  double distance_square{ 0. };
  double min_distance{ std::numeric_limits<double>::max() };
  std::pair<size_t, double> currentCluster_and_minDistanceSquare;

  double total_distance{ 0. };
  double min_total_distance{ std::numeric_limits<double>::max() };
  double percentage_change{ 0 };

  int iteration{ -1 };
  short consecutive_unchanged{ 0 };

  std::chrono::high_resolution_clock::time_point start_cluster_assign;
  std::chrono::high_resolution_clock::time_point end_cluster_assign;

  //
  // STEP ONE: select k=10 cluster centers
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
      min_distance = std::numeric_limits<double>::max();

      currentCluster_and_minDistanceSquare = determine_cluster(_cluster_centers, _training_data[p], _count_clusters);

      _point_to_cluster[p] = currentCluster_and_minDistanceSquare.first;
      _cluster_to_point[ currentCluster_and_minDistanceSquare.first ][ _cluster_to_point_current_sizes[currentCluster_and_minDistanceSquare.first]++ ] = p;

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
        << '\n';

      if (++consecutive_unchanged == _training_exit_on_unchanged_error)
      {
        output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_CONSECUTIVE_UNCHANGED\n";
        break;
      }
    }
    else
    {
      output
        << iteration
        << '|' << std::chrono::duration_cast<std::chrono::milliseconds>(end_cluster_assign - start_cluster_assign).count()
        << '|' << percentage_change
        << '|' << total_distance
        << '|' << (iteration ? min_total_distance : 0.)
        << '\n';

      output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_MIN_TOTAL_DISTANCE_INCREASED\n";
      break;
    }

    //
    // STEP THREE: new clusters
    //
    // new center of cluster: each element of feature vector is the mean of current cluster points
    //

    for (size_t xcount, c{ 0 }; c < _count_clusters; c++)
    {
      _cluster_centers[c] = 0.;
      xcount = _cluster_to_point_current_sizes[c];
      for (size_t x{ 0 }; x < xcount; x++)
        _cluster_centers[c] += _training_data[ _cluster_to_point[c][x] ];
      _cluster_centers[c] /= static_cast<double>(xcount);
    }

    //
    //
    //

    _cluster_to_point_current_sizes.assign(_count_clusters, 0);

    distance = total_distance = percentage_change = 0.;
  }

  if (iteration == _training_exit_on_iterations)
  {
    output << "KMeansClustering::KMeansClustering(): i_" << iteration << ": EXIT_CONDITION_MAX_ITERATIONS\n";
  }

  //
  //
  //

//#define xxx_validate_clusters
#ifdef xxx_validate_clusters
  validate_clusters(output, _count_clusters, _point_to_cluster, _cluster_to_point_current_sizes, _cluster_to_point, _training_labels);
#endif

  //
  //
  //

  auto out = std::chrono::high_resolution_clock::now();

  output << "KMeansClustering::KMeansClustering(): out " << std::chrono::duration_cast<std::chrono::milliseconds>(out - in).count() << " ms\n";
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

  std::vector<std::valarray<double>> test_data{ std::move(test_dataset).scaledFeatureVectors() };
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
    currentCluster_and_minDistanceSquare = determine_cluster(_cluster_centers, test_data[iTestData], _count_clusters);

    output

      << "TEST_" << (iTestData + 1)

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::STATUS][OUTPUT_MODE]
      << P_STR_OK_NOK[count_success++, 0]

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::EXPECTED][OUTPUT_MODE]
      << static_cast<int>(test_labels[iTestData])

      << KMeansClustering::P_STR_ARRAY_2[KMeansClustering::RESULT][OUTPUT_MODE]
      << currentCluster_and_minDistanceSquare.first

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
