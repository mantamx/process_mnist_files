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
Implementing any of the approaches requires [reading and handling MNIST files](./mnist_idx_ubyte/README_MNIST_IDX.md), implemented in `class MnistHandler` and `class MnistDataset`, respectively. I think they are good and final for all of the approaches. In case I'm wrong, necessary changes will have to follow along the way.

[K Nearest Neighbors](./k_nearest_neighbors/README_KNN.md), the easiest approach, is implemented as `class KNearestNeighbors`.

[K-Means Clustering](./k_means_clustering/README_KMC.md) is implemented as `class KMeansclustering`.

Implementation of [Naïve Bayes Classifier](./naive_bayes_classifier/README_NBC.md) as `class NaiveBayesClassifier` has started, but nothing is checked in yet.

In folder EXAMPLES, `main_knn.cpp`, `main_kmc.cpp`, and `main_mnist_handler.cpp` are [example programs](./EXAMPLES/README_EXAMPLES.md) that are sufficiently parameterized to experiment with the above classes.

Implementation wise, NaiveBayesClassifier is set up, but I still lack the level of understanding I need in some parts in order to autonomously implement NBC. Handling the counts and probabilities <!-- (though, probabilities will not be used in final calculations) --> is OK, but I'm trying to come to terms with how to deal with feature values of test images that have not occurred in the training set.
I know to apply a **Laplace correction** for these zero counts, as well as the need to scale the data. But am yet unsure about a few aspects around this.

In parallel, while thinking on Naïve Bayes Classifier, I am also looking into understanding neural networks and have come quite far. This will help me in implementation of a basic and conventional neural network with perceptrons and a Sigmoid or ReLU activation function. I know the final layer must have 10 output nodes, the first layer has 784 inputs. Using back propagation to determine the weights and biases of the negative gradient of the cost function (in order to find the minimum of the cost function) is a welcome challenge that essentially boils down to structuring things properly.

By the way, the approach taken for the cost function for Neural Networks is the same total distance calculation that I have considered in my implementation of K-Means Clustering. With K-Means Clustering, a value is calculated as the sum of differences of Eucledean distances, and over iterations in which centroids are recalculated, the percentage changes of this decreasing total distance / error / cost is tracked, and it is one of few considered critieria to break out of the loop. With Neural Networks the cost function has dependencies on lots of weights and biases (the parameters of the Neural Network). Therefore, it is a function with terms we have to estimate in order to find the minimum (or rather the smallest of a series of minima).
