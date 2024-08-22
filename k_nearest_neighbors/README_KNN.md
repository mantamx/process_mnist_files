## K Nearest Neighbors
There is no training involved with KNN. It is computationally intensive as it calculates distances from a test point to the reference data, over and over, with nothing to cache.

Therefore argument `-count_train` (see `main_knn.cpp`) has runtime impact.

Also the distribution of points (images, each a vector in 784-dimensional space) plays a significant role in the outcome of the test. Due to the way I chose to process the data and model the dataset, I can't introduce an optional parameter to shuffle the training (or what I called reference) data. This can be changed, but it's not necessary as I now understand the algorithm and know the effect of the distribution on the outcome. A way to compensate for this effect / dependency is, in my view, to increase `-k`.
