# Motivation
Idea was to get into AI a little bit, starting with basics, in C++. Cracking down on neural networks is my dream since quite a while. Yet, with no clue of a reasonable start and what to do in form of a suitable test project I was lost before even embarking on the jourmey.

Came accross MNIST files through **_gtaylor5_**. This saved me, because after one video of the series I had plenty of threads and insight. My brain was enthusiastically firing and I had to take off! Given the blueprint, I decided to write my implementation in order to understand what and where the challenges are.

Therefore, my task / test project: implement various approaches / techniques for interpreting handwritten digits using MNIST datasets.
1. K Nearest Neighbors
2. K-Means Clustering
3. Naïve Bayes Clustering
4. Neural Network

I'm learning as I progress through the points, and it's great! The dream I mentioned has turned into a plan, because I am taking action, finally.
## Implementation
For now, K Nearest Neighbors, the easiest approach, is implemented as `class KNearestNeighbors`, and I have started with K-Means Clustering.

Implementing any of the approaches requires reading the MNIST files, `class MnistHandler`, and handling the data, `class MnistDataset`. I think they are good and final for all of the approaches. In case I'm wrong, necessary changes will have to follow along the way.

## K Nearest Neighbors
There is no training involved with KNN. It is computationally intensive as it calculates distances from a test point to the backing data, over and over, with nothing to cache, etc.

Therefore argument `-count_train` (see EXAMPLE.exe) has runtime impact.

Also the distribution of points (images, each a vector in 784-dimensional space) plays a significant role in the outcome of the test. Due to the way I chose to handle / model the data, I don't have the option to introduce optional parameter to shuffle the training (or what I called backing) data. Can change this, but it's not necessary as I now understand the algorithm and know the effect of the distribution on the outcome. A way to compensate for this effect / dependency is, in my view, to increase <-k>.

# EXAMPLE.exe
This is my playground, and it helped me a lot to feel the intrinsics of KNN by comparing results with changing arguments.
The parameters of this program will change as the remaining approaches are implemented.
## Parameters of main()
Parameters are in the form name=value. The order of the arguments in the call to main() is not relevant for their processing, and therefore name= must be provided for the argument to be identified. Some parameters have default values.
|Name|Interpretation of value|
|:---|:---|
|-path_train_images|string, path to MNIST binary file with training image data|
|-path_train_labels|string, path to training file with labels|
|-path_test_images|string, path to test file with images|
|-path_test_labels|string, path to test file with labels|
|-k|integer, specific to **_K Nearest Neighbors_**, defaults to 3, range: 1...<-count_train>|
|-count_train|integer, number of records from **_training data_** to consider for calculations, defaults to 10000|
|-count_test|integer, number of records from **_test data_** to test, defaults to 10|
|-output_mode|string, defaults to 'delimited', range: 'delimited', 'pretty'|

## Arguments of the run
```
.\EXAMPLE.exe path_train_images=..\..\..\MNIST_DATA\train-images.idx3-ubyte path_train_labels=..\..\..\MNIST_DATA\train-labels.idx1-ubyte path_test_images=..\..\..\MNIST_DATA\t10k-images.idx3-ubyte path_test_labels=..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte k=3 count_train=5000 count_test=50 output_mode=delimited
```
|Name|Value|
|:---|:---|
|-path_train_images|..\..\..\MNIST_DATA\train-images.idx3-ubyte|
|-path_train_labels|..\..\..\MNIST_DATA\train-labels.idx1-ubyte|
|-path_test_images|..\..\..\MNIST_DATA\t10k-images.idx3-ubyte|
|-path_test_labels|..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte|
|-k|3|
|-count_train|5000|
|-count_test|50|
|-output_mode|delimited|
## Output of the run
```
main(): in
main(): reading 4 files: training images, training labels, test images, test labels...
main(): ...4 files read [43 ms]

main(): skip header, print 32 bytes starting with 159th byte, 4 bytes per line, with line numbers

10101111 00011010 10100110 11111111  : 1
11110111 01111111 00000000 00000000  : 2
00000000 00000000 00000000 00000000  : 3
00000000 00000000 00000000 00000000  : 4
00000000 00000000 00011110 00100100  : 5
01011110 10011010 10101010 11111101  : 6
11111101 11111101 11111101 11111101  : 7
11100001 10101100 11111101 11110010  : 8

main(): pretty-print the same: 0-bytes as space, otherwise asterix

* * * *  : 1
* *      : 2
         : 3
         : 4
    * *  : 5
* * * *  : 6
* * * *  : 7
* * * *  : 8

main(): TRAIN_IMAGES header values...
main(): magic number: 2051
main(): item count: 60000
main(): row count: 28
main(): column count: 28

main(): TRAIN_LABELS header values...
main(): magic number: 2049
main(): item count: 60000

main(): TEST_IMAGES header values...
main(): magic number: 2051
main(): item count: 10000
main(): row count: 28
main(): column count: 28

main(): TEST_LABELS header values...
main(): magic number: 2049
main(): item count: 10000

main(): TRAIN_LABELS labels 0, 11 and 40...
5
5
1

main(): TEST_LABELS labels 0, 11 and 40...
7
6
1

main(): TRAIN_IMAGES images 0, 11 and 40...
                                                        
                                                        
                                                        
                                                        
                                                        
                        * * * * * * * * * * * *         
                * * * * * * * * * * * * * * * *         
              * * * * * * * * * * * * * * * *           
              * * * * * * * * * * *                     
                * * * * * * *   * *                     
                  * * * * *                             
                      * * * *                           
                      * * * *                           
                        * * * * * *                     
                          * * * * * *                   
                            * * * * * *                 
                              * * * * *                 
                                  * * * *               
                            * * * * * * *               
                        * * * * * * * *                 
                    * * * * * * * * *                   
                * * * * * * * * * *                     
            * * * * * * * * * *                         
        * * * * * * * * * *                             
        * * * * * * * *                                 
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                        * * * * * *     
                                    * * * * * * * *     
                                * * * * * * * * *       
                            * * * * * * * *             
                            * * * *                     
                      * * * * * *                       
                      * * * *                           
                      * * *                             
                      * * * *                           
                * *   * * * * *                         
            * * * *     * * * *                         
            * * *       * * * *                         
            * * * * * * * * *                           
              * * * * * * * *                           
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                            * * *                       
                          * * * * *                     
                          * * * * *                     
                          * * * * *                     
                          * * * *                       
                          * * * *                       
                          * * * *                       
                          * * * *                       
                          * * * *                       
                        * * * * *                       
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * * *                         
                        * * *                           
                                                        
                                                        
                                                        

main(): TEST_IMAGES images 0, 11 and 40...
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
            * * * * * *                                 
            * * * * * * * * * * * * * * * *             
            * * * * * * * * * * * * * * * *             
                      * * * * * * * * * * *             
                                    * * * *             
                                  * * * *               
                                  * * * *               
                                * * * *                 
                                * * * *                 
                              * * * *                   
                              * * *                     
                            * * * *                     
                          * * * *                       
                        * * * * *                       
                        * * * *                         
                      * * * * *                         
                      * * * *                           
                    * * * * *                           
                    * * * * *                           
                    * * * *                             
                                                        
                                                        
                                                        
                                                        
                    * * * *                             
                  * * * * *                             
                  * * * *                               
                  * * *                                 
                  * * *                                 
                * * *                                   
                * * *               * * * *             
                * * *           * * * * * * *           
              * * *       * * * * * * * * * * *         
              * * *     * * * * * * *     * * *         
              * * *     * * * * *         * * *         
              * * *   * * * * *           * * *         
              * * *   * * *               * *           
              * * *   * * *             * * *           
              * * *   * * * * *         * * *           
            * * * *     * * * * *   * * * *             
              * * * *       * * * * * * *               
              * * * * * * * * * * * * *                 
                * * * * * * * * * * *                   
                  * * * * * * * *                       
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                                                        
                          * *                           
                          * *                           
                          * *                           
                          * *                           
                          * * *                         
                            * *                         
                            * *                         
                            * *                         
                            * *                         
                            * *                         
                          * * *                         
                          * * *                         
                          * * *                         
                          * *                           
                          * *                           
                          * *                           
                          * *                           
                        * * *                           
                        * * *                           
                        * *                             
                                                        
                                                        
                                                        
                                                        

main(): instantiating knn object with normalized training data...
main(): ...knn object instantiated [485 ms]
main(): running test with normalized test data...
KNearestNeighbors::runTest(): in
KNearestNeighbors::runTest(): TEST_START:
	k=3
	training_data=5000
	test_data=50
Test|Status|Expected|Result|Count|Distance|Duration
TEST_1|OK|7|7|4|4.297488 4.303122 4.572342|51 ms
TEST_2|OK|2|2|4|6.121553 6.550226 7.185645|49 ms
TEST_3|OK|1|1|4|2.145482 2.375153 2.529640|51 ms
TEST_4|OK|0|0|4|5.089359 5.115875 5.480296|47 ms
TEST_5|OK|4|4|3|5.186302 5.237300 5.527879|46 ms
TEST_6|OK|1|1|4|1.462908 1.910822 2.124868|48 ms
TEST_7|OK|4|4|3|5.840947 6.005443 6.286732|46 ms
TEST_8|OK|9|9|4|6.099906 6.153502 6.606382|46 ms
TEST_9|OK|5|5|4|8.044861 8.127909 8.501956|48 ms
TEST_10|OK|9|9|4|4.905771 5.130699 5.575049|48 ms
TEST_11|OK|0|0|4|4.471668 4.689057 5.573836|55 ms
TEST_12|OK|6|6|4|6.491079 6.659940 7.324771|49 ms
TEST_13|OK|9|9|4|3.539737 3.827855 4.295249|47 ms
TEST_14|OK|0|0|4|3.757366 5.040243 5.445808|48 ms
TEST_15|OK|1|1|4|2.492252 2.553772 2.914340|43 ms
TEST_16|OK|5|5|4|6.364414 6.589044 6.859559|48 ms
TEST_17|OK|9|9|3|4.648328 4.819930 5.145173|50 ms
TEST_18|OK|7|7|4|4.646653 4.912968 5.198533|47 ms
TEST_19|OK|3|3|4|7.472582 8.427665 8.510137|51 ms
TEST_20|OK|4|4|4|3.962933 4.053201 4.342028|47 ms
TEST_21|OK|9|9|4|4.302878 4.501354 4.587317|45 ms
TEST_22|OK|6|6|4|4.875119 5.055512 5.567209|44 ms
TEST_23|OK|6|6|4|5.692452 6.024515 6.967512|46 ms
TEST_24|OK|5|5|4|5.240033 5.375215 5.675913|46 ms
TEST_25|NOK|4|9|3|4.563778 4.648925 5.511833|45 ms
TEST_26|OK|0|0|4|6.442389 6.469933 6.721232|52 ms
TEST_27|OK|7|7|4|4.105380 4.181578 4.868354|46 ms
TEST_28|OK|4|4|4|4.515115 4.520179 4.690135|51 ms
TEST_29|OK|0|0|4|5.284938 5.307717 5.608134|50 ms
TEST_30|OK|1|1|4|2.466978 2.603555 2.854266|48 ms
TEST_31|OK|3|3|4|5.452100 5.549013 6.133506|50 ms
TEST_32|OK|1|1|4|2.151648 2.384459 2.846194|47 ms
TEST_33|OK|3|3|4|4.918635 5.035085 5.550109|53 ms
TEST_34|OK|4|4|3|7.719063 7.832345 8.061908|51 ms
TEST_35|OK|7|7|4|3.552805 4.055440 4.580088|47 ms
TEST_36|OK|2|2|4|7.406676 7.488821 7.697243|48 ms
TEST_37|OK|7|7|4|5.236089 5.252931 5.429801|49 ms
TEST_38|OK|1|1|4|1.612366 1.676896 2.150193|45 ms
TEST_39|OK|2|2|3|6.289125 6.716333 6.823007|48 ms
TEST_40|OK|1|1|4|2.875005 2.906364 3.135262|49 ms
TEST_41|OK|1|1|4|2.218440 2.228404 2.641743|46 ms
TEST_42|OK|7|7|4|4.043702 4.556346 4.812568|47 ms
TEST_43|OK|4|4|4|4.812052 5.024737 5.214409|47 ms
TEST_44|NOK|2|1|4|6.309667 6.536674 6.619574|47 ms
TEST_45|NOK|3|5|1|5.618592 5.895691 6.164666|46 ms
TEST_46|OK|5|5|3|5.729671 5.911932 5.970106|63 ms
TEST_47|OK|1|1|4|2.972942 3.402625 3.793434|54 ms
TEST_48|OK|2|2|4|5.145636 5.572339 6.310497|47 ms
TEST_49|OK|4|4|4|5.951755 6.229398 6.322020|49 ms
TEST_50|OK|4|4|4|5.591473 5.768738 5.813098|47 ms
KNearestNeighbors::runTest(): TEST_DONE:
	k=3
	training_data=5000
	test_data=50
	error=3
	success=47
	pre_test=26
	test=2445
	post_test=0
KNearestNeighbors::runTest(): out
main(): ...test done, knn function call with test data [2536 ms]

main(): out
```
