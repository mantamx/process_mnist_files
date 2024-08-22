## K-Means Clustering
Training is involved. Actually, it is calculating cluster centers. For this implementation, the number of clusters is parameterized, but I have sofar tested mainly with 10.

As with K Nearest Neighbors, the EXAMPLE app is sufficiently parameterized to allow for playing around with arguments in order to have a better feeling. So far my testing has been to verify the functionality is correct. To better establish a feeling for interaction of arguments in run-time and outcome, I need to play around more.

Training can take quite a long time depending on parameter values (for example, number of clusters, number of training points, and break conditions). Testing is very quick.

Overview of the training approach:

0. **USE TRAINING DATA**. Image data for the calculations, labels for setting clusters as the final step of the training. (After this, the clusters are ready for testing.)
1. **SET_K**
2. **DETERMINE INITIAL CLUSTER POINTS (QUASI CENTROIDS)**. In my case, I select one data point per bucket, and there are K buckets.
    ```
    for (size_t i{ 0 }; i < _count_clusters; i++)
    {
      _cluster_centers[i] = _training_data[ ((_count_training_data / _count_clusters) >> 1) + i * _count_training_data / _count_clusters ];
    }
    ```
4. **REPEAT**
    1. **ASSIGN CLUSTER**. Determine which cluster each point (vector of features) belongs to by calculating its Eucledean distance (vector) to each cluster. The cluster to which the distance is smallest, is the one.
    2. **CHECK BREAK CONDITIONS**
        1. Percentage change in total distance is less than a prameterized value. **_Total distance_**: Calculate the distance of each point to the center of the cluster it is assigned to, which is by definition the minimum of all the point's distances to the K clusters. Square this distance. Sum for all points. This is a measure of total distance or error, and it is a decreasing function of k, with the minimum value of 0 when K equals the number of points (each data point would be centroid of a cluster comprised of only that point). **_A note on percentage change_**: Given the algorithm of determining the new cluster centers, it is guaranteed that the total distance for a given K never increases accross the iterations. However, this does not apply to the percentage change from one iteration to the next.
        2. Maximum number of iterations (parameter) is reached.
        3. The total distance is unchanged for n (parameter) consecutive iterations.
    3. **DETERMINE NEW (REAL) CENTROIDS**. The mean of the points assigned to a cluster is calculated, and this mean is the new center, the centroid.
5. **SET_CLUSTER**. Determine the most frequent class among the points that are assigned to each cluster as of the last iteration (state directly before a break clause kicked in). This is where the labels are used. **_A note on setting clusters and most frequent classes_**: Given the example of K=10 clusters for digits 0 through 9, when setting the clusters, it is not guaranteed that each of the 10 clusters **clearly** or at all represents a class. That is, can be (and in it came up in my tests as an issue that I have to address) that merely based on most frequent class numbers, a class is represented by more than one cluster (collision), thus leaving classes that are not represented by any of the 10 clusters. That is, K has to be incremented by the number of collisions. But another issue can come up: The most frequent class may not be a clear representative by an acceptable parameterized percentage. For example, if 1000 points are assigned to a cluster and the most frequent class occurs 950 times, then it is a quite good and safe outcome that the cluster represents the most frequent class. But if the first most frequent class occurs 500 times, and the second 400, then it poses an issue when testing and classifying based on the cluster assignment resulting from the training. I found that frequencies of 4|9, 7|9, and 5|3 were not sufficiently apart, and this clearly resonated in the subsequent tests `KMeansClustering::runTest()`. **At the time of this update I'm taking a break from this to clear out, but will soon consolidate my thoughts for an approach to a solution**. **_SEE UPDATE BELOW_**.

**_UPDATE_**

It seems that the solution is simply to add clusters. Although, _simply_ entails increased processing time for all the calculations when training / defining and later using the clusters. What surprised me a lot was that with a cluster count of 200 the 10000 test points could be classified with 89% accuracy. Setting clusters to 400 did not change much. Testing the 10000 points is comparatively lightning fast.

The times in the table below provide only a rough comparison as my notebook running Windows was not always entirely dedicated to the tests. For example, the test in the 3rd listed run took 12% longer to complete than the second run, although both are based on 200 clusters.

A final point that I use an optimization by determining which features are zero accross all points of the training set. With more points in the training set the number of common zero-features tends to decline. Centroid calculations are skipped for these features. For 20000 training points, 88 of the 784 features are zero accross all the points. For 40000 training points it is 71.

|Training data|Clusters|Train dur [ms]|Loop exit|Loop iter|Test data|Test dur [ms]|Success|Error|
|---:|---:|---:|:---:|---:|---:|---:|---:|---:|
|20000|400|7399011|pct_chg_err|34|10000|57297|9095|905|
|20000|200|3428342|max_iter|50|10000|31810|8922|1078|
|40000|200|10271340|max_iter|50|10000|35630|9073|927|
|20000|500|7133624|pct_chg_err|48|10000|76539|9185|815|
|20000|1000|8054173|pct_chg_err|26|10000|244123|9277|723|

Comparison with KNN. Although the training of KMC takes quite some time, the testing is a walk in the park in terms of number of calculations. Seemingly, this beats KNN, at least according to my tests. However, considering accuracy in identifying the digits does change the odds in my view. KNN reaches 95% accuracy with 20000 training points and 3 neighbors. KMC with 20000 training points and 200 clusters is faster with 75% of the KNN time, but identifies only 89% of the digits correctly. Even with 1000 clusters, KMC reached only almost 93% accuracy, but in double the KNN time.

Admittedly, my tests are not by any means a benchmark, but I think they still provide good guidance. Depending on the level of required accuracy and the extent of the training data, one approach may be preferable over the other.

|Training data|Neighbors|Test data|Test duration [ms]|Success|Error|
|---:|---:|---:|---:|---:|---:|
|20000|3|10000|4599585|9565|435|
