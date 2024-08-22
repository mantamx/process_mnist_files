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
