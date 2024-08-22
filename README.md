# Motivation
Idea was to get hands-on experience in AI, with C++. Cracking down on neural networks is my dream since quite a while. Yet, with no clue of what to do in form of a suitable test project I was lost before even embarking on the jourmey.

Came accross MNIST files through **_gtaylor5_**. This saved me, because after one video of the series I had plenty of threads and insight. My brain was enthusiastically firing and I had to take off! Given the blueprint, I decided to have my design and implementation in order to understand what and where the challenges are.

Therefore, my first task is a test project, in which I implement a few approaches of interpreting handwritten digits based on the MNIST files.
1. K Nearest Neighbors
2. K-Means Clustering
3. Naïve Bayes Classifier
4. Neural Network

I'm learning as I progress through the points, and it's great! The dream I mentioned has turned into a plan, because I am taking action, finally.
## Current State of Implementation
Implementing any of the approaches requires reading the MNIST files, `class MnistHandler`, and handling the data, `class MnistDataset`. I think they are good and final for all of the approaches. In case I'm wrong, necessary changes will have to follow along the way.

K Nearest Neighbors, the easiest approach, is implemented as `class KNearestNeighbors`.

K-Means Clustering is implemented as `class KMeansclustering`.

Implementation of Naïve Bayes Classifier as `class NaiveBayesClassifier` has started, but nothing is checked in yet.

`main_knn.cpp`, `main_kmc.cpp`, and `main_mnist_handler.cpp` are [example programs](./EXAMPLE/README_EXAMPLES.md), sufficiently parameterized to experiment with the above classes.

_Implementation wise, NaiveBayesClassifier is set up, but I still lack the level of understanding I need in some parts in order to autonomously implement NBC. Handling the counts and probabilities <!-- (though, probabilities will not be used in final calculations) --> is OK, but I'm trying to come to terms with how to deal with feature values of test images that have not occurred in the training set. I know to apply a Laplace correction for these zero counts, as well as the need to scale the data. But am yet unsure about a few aspects around this._

_In parallel, while thinking on Naïve Bayes Classifier, I am also looking into understanding neural networks and have come quite far. This will help me in implementation of a basic and conventional neural network with perceptrons and a Sigmoid or ReLU activation function. I know the final layer must have 10 output nodes, the first layer has 784 inputs. Using back propagation to determine the weights and biases of the negative gradient of the cost function (in order to find the minimum of the cost function) is a welcome challenge that essentially boils down to structuring things properly._

_By the way, the approach taken for the cost function for Neural Networks is the same total distance calculation that I have considered in my implementation of K-Means Clustering. With K-Means Clustering, a value is calculated as the sum of differences of Eucledean distances, and over iterations in which centroids are recalculated, the percentage changes of this decreasing total distance / error / cost is tracked, and it is one of few considered critieria to break out of the loop. With Neural Networks the cost function has dependencies on lots of weights and biases (the parameters of the Neural Network). Therefore, it is a function with terms we have to estimate in order to find the minimum (or rather the smallest of a series of minima)._

## K Nearest Neighbors
There is no training involved with KNN. It is computationally intensive as it calculates distances from a test point to the reference data, over and over, with nothing to cache.

Therefore argument `-count_train` (see `main_knn.cpp`) has runtime impact.

Also the distribution of points (images, each a vector in 784-dimensional space) plays a significant role in the outcome of the test. Due to the way I chose to process the data and model the dataset, I can't introduce an optional parameter to shuffle the training (or what I called reference) data. This can be changed, but it's not necessary as I now understand the algorithm and know the effect of the distribution on the outcome. A way to compensate for this effect / dependency is, in my view, to increase `-k`.

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

## Naïve Bayes Classifier
We want to classify an image in the test set, with given features. That is, we want to predict its class. Input is the test point's feature vector. With this input we work off the training set and calculate for each possible class C in the training set, the probability of that input feature vector belonging to C. The highest of such calculated probabilities determines the class we choose as prediction.

**The main question to ask:**
What is the probability of an image with these features being a 0. And this question has to be asked 10 times (for 0 through 9).
What is the probability of an image with these features being a 0. And this question has to be asked for classes 0 through 9.

**How to go about calculating the answer to one such question?**

Bayes theorem + the naive assumption that the features are independent. This is the Naive Bayes Classifier.

<Probability that the point is 0, given that it has this feature vector (test set)> x <Probability of that feature vector occurring (in training set)> = <Probability that this feature vector is among the 0's (in training set)> x <Probability of 0's (in training set)>

This goes for all points 0 through 9. That is, for each point in the test set, 10 calculations / comparisons are carried out, and one of them is chosen as the prediction for the class of that point.

**The same statement or equation with conditional aspects more clearly formulated:**

P( point is c | {x<sub>i</sub>} ) x P( {x<sub>i</sub>} ) = P( {x<sub>i</sub>} | c ) x P( c ), for c := {0..9}

**Simplified:**

P( Class_c | Test_X<sub>i</sub> ) x P( Training_X<sub>i</sub> ) = P( Training_X<sub>i</sub> | Class_c ) x P( Class_c )

**Notes:**

P( Class_c ) is constant for each specific c value in the training set.

P( Training_X<sub>i</sub> ) is not constant, but common for the calculations for determining the class c.

The Class_c is what we want to determine, given the point (feature vector) X<sub>i</sub> of the test set.

Based on this feature vector X<sub>i</sub> from the TEST set, we iterate over the TRAINING set.

P( X<sub>i</sub> ) can be ignored in the comparisons of 0..9 for the given X<sub>i</sub>, because it is common to all Class_0...9 calculations (and we are only comparing the numbers for each class, the exact number doesn't matter).

We have given feature vector X<sub>i</sub>, test for 0..9 by calculating P( Training_X<sub>i</sub> | Class_0..9 ) off the TRAINING set multiplied by the constants P( Class_0..9 ).

In this calculation, the naive assumption of independence of the individual features X_i in the feature vector simplifies calculations by allowing us to (a) 'independently' calculate probability for each feature, and (b) multiply these probabilities with each other.

We do this for all classes 0...9.

The highest P( Training_X<sub>i</sub> | Class_c ) x P( Class_c ) is the prediction we choose.

**Concerns:**

Q: What happens if a feature A<sub>i</sub> does not occur in the TRAINING set? (The probability of zero should not invalidate this feature's calculation.)

A: Laplace correction

## Neural Network
_NOT STARTED
