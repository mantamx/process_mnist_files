# Motivation
Have one class to read both file types, image and labels. Their format is the same, only interpretation varies.
`class MnistHandler` accomplishes this. It supports move semantics. Copy constuctor and assignment operator are disabled. (This is to promote moving data as opposed to copying it. If a handler is to be used in more than one utilizing class, then a new object can be instantiated with the same arguments.)

The implementation of the MNIST handler delegates printing bits to the free function defined in `./print_bits/print_bits.cpp`. `./EXAMPLES/main_mnist_handler.cpp` is an [example](../EXAMPLES/README_EXAMPLES.md#example_mnist_handler) of how this function is used.

Once the data is read, it is intended to be used to create datasets. The handler retains image and label data formats. Dataset classes on the other hand create objects off the handler that are ready to be further processed by classes for [K Nearest Neighbors](../k_nearest_neighbors/README_KNN.md), [K-Means Clustering](../k_means_clustering/README_KMC.md), etc.

At present `class MnistDatasetNormalized` and `class MnistDatasetStandardized` are defined, both deriving from `class MnistDatasetBase`. These classes are generic in the sense that they simply scale the data.
However, my current understanding is that for [Naïve Bayes Classifier](../naive_bayes_classifier/README_NBC.md) a new, specific class is required instead.
This class applies [Laplace correction](../README.md#nbc-and-nn) to the data prior to scaling.
