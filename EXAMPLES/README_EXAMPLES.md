# EXAMPLES
These are my playgrounds, and they helped me a lot to feel the intrinsics of KNN by comparing results with changing arguments.
Initially, there was only one binary in the folder EXAMPLE. But due to developments on K-Means Clustering, I found it better to have separate examples, still all in the folder EXAMPLES.
Hence a series of `main_*.cpp` files. Plus, to streamline handling of arguments, their respective checks, and generation of call syntax, arg.hpp is new.

* main_mnist_handler.cpp
* main_knn.cpp
* main_kmc.cpp

## EXAMPLE_MNIST_HANDLER

## Parameters of main()
Parameters are in the form name=value. The order of the arguments in the call to main() is not relevant for their processing, and therefore name= must be provided for the argument to be identified. Some parameters have default values.

|Name|Description|
|:---|:---|
|path_train_images|interpreted as string; path to MNIST binary file with training image data|
|path_train_labels|interpreted as string; path to training file with labels|
|path_test_images|interpreted as string; path to MNIST binary file with test image data|
|path_test_labels|interpreted as string; path to test file with labels|

## Arguments of the run
```
.\EXAMPLE_MNIST_HANDLER.exe path_train_images=..\..\..\MNIST_DATA\train-images.idx3-ubyte path_train_labels=..\..\..\MNIST_DATA\train-labels.idx1-ubyte path_test_images=..\..\..\MNIST_DATA\t10k-images.idx3-ubyte path_test_labels=..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte
```
|Name|Value|
|:---|:---|
|path_train_images|..\\..\\..\MNIST_DATA\train-images.idx3-ubyte|
|path_train_labels|..\\..\\..\MNIST_DATA\train-labels.idx1-ubyte|
|path_test_images|..\\..\\..\MNIST_DATA\t10k-images.idx3-ubyte|
|path_test_labels|..\\..\\..\MNIST_DATA\t10k-labels.idx1-ubyte|

## Output of the run, Windows release build
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
                                                        
                                                        
                                                        
                                                        
main(): out
```

## EXAMPLE_KNN

## Parameters of main()

|Name|Description|
|:---|:---|
|path_train_images|interpreted as string; path to MNIST binary file with training image data|
|path_train_labels|interpreted as string; path to training file with labels|
|path_test_images|interpreted as string; path to MNIST binary file with test image data|
|path_test_labels|interpreted as string; path to test file with labels|
|k|interpreted as integer; defaults to 3, range: 1...<train_count>|
|train_count|interpretd as integer; number of records from training data to consider for calculations, defaults to 20000|
|test_count|interpreted as integer; number of records from test data to test, defaults to 50|
|output_mode|interpreted as string; defaults to "delimited", range: "delimited", "pretty"|

## Arguments of the run
This is the command line:
```
.\EXAMPLE_KNN.exe path_train_images=..\..\..\MNIST_DATA\train-images.idx3-ubyte path_train_labels=..\..\..\MNIST_DATA\train-labels.idx1-ubyte path_test_images=..\..\..\MNIST_DATA\t10k-images.idx3-ubyte path_test_labels=..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte train_count=5000 test_count=50
```
6 of the 8 arguments provided in command line. The remaining 2 are defaulted.
The check routine passed as a lambda expression to an instance of `ArgumentHandler` (see arg.hpp), assigns and logs the defaulted values:
```
argument check: setting 'k' to default 3
argument check: setting 'output_mode' to default 'delimited'
```
This is the corresponding code:
```
int main(int argument_count, char** arguments)
{
  std::cout << "main(): in" << std::endl;

  auto myCheck =
    [](std::array<ArgNameValue, MAX_COUNT_ARGS>& args)
    {
      //
      // check arguments
      //

      if (args[K].value.length() == 0)
      {
        args[K].value = "3";
        std::cout << "argument check: setting \'" << args[K].name << "\' to default 3\n";
      }

      if (args[TRAIN_COUNT].value.length() == 0)
      {
        args[TRAIN_COUNT].value = "10000";
        std::cout << "argument check: setting \'" << args[TRAIN_COUNT].name << "\' to default 10000\n";
      }

      if (args[TEST_COUNT].value.length() == 0)
      {
        args[TEST_COUNT].value = "10";
        std::cout << "argument check: setting \'" << args[TEST_COUNT].name << "\' to default 10\n";
      }

      if (args[OUTPUT_MODE].value.length() == 0)
      {
        args[OUTPUT_MODE].value = "delimited";
        std::cout << "argument check: setting \'" << args[OUTPUT_MODE].name << "\' to default \'delimited\'\n";
      }
    };

  ArgumentHandler argument_handler
  (
    argument_count
    , arguments
    , std::move(ARG_NAME)
    , myCheck
  );
```

|Name|Value|
|:---|:---|
|path_train_images|..\\..\\..\MNIST_DATA\train-images.idx3-ubyte|
|path_train_labels|..\\..\\..\MNIST_DATA\train-labels.idx1-ubyte|
|path_test_images|..\\..\\..\MNIST_DATA\t10k-images.idx3-ubyte|
|path_test_labels|..\\..\\..\MNIST_DATA\t10k-labels.idx1-ubyte|
|count_train|5000|
|count_test|50|

## Output of the run, Windows release build
```
main(): in
argument check: setting 'k' to default 3
argument check: setting 'output_mode' to default 'delimited'
main(): arg_0: path_train_images|..\..\..\MNIST_DATA\train-images.idx3-ubyte
main(): arg_1: path_train_labels|..\..\..\MNIST_DATA\train-labels.idx1-ubyte
main(): arg_2: path_test_images|..\..\..\MNIST_DATA\t10k-images.idx3-ubyte
main(): arg_3: path_test_labels|..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte
main(): arg_4: k|3
main(): arg_5: train_count|5000
main(): arg_6: test_count|50
main(): arg_7: output_mode|delimited
main(): reading 4 files: training images, training labels, test images, test labels...
main(): ...4 files read [38 ms]
main(): instantiating knn object with normalized training data...
main(): ...knn object instantiated [304 ms]
main(): running test with normalized test data...
KNearestNeighbors::runTest(): in
KNearestNeighbors::runTest(): TEST_START:
        k=3
        training_data=5000
        test_data=50
Test|Status|Expected|Result|Count|Distance|Duration
TEST_1|OK|7|7|4|4.297488 4.303122 4.572342|43 ms
TEST_2|OK|2|2|4|6.121553 6.550226 7.185645|45 ms
TEST_3|OK|1|1|4|2.145482 2.375153 2.529640|45 ms
TEST_4|OK|0|0|4|5.089359 5.115875 5.480296|51 ms
TEST_5|OK|4|4|3|5.186302 5.237300 5.527879|45 ms
TEST_6|OK|1|1|4|1.462908 1.910822 2.124868|43 ms
TEST_7|OK|4|4|3|5.840947 6.005443 6.286732|44 ms
TEST_8|OK|9|9|4|6.099906 6.153502 6.606382|44 ms
TEST_9|OK|5|5|4|8.044861 8.127909 8.501956|46 ms
TEST_10|OK|9|9|4|4.905771 5.130699 5.575049|45 ms
TEST_11|OK|0|0|4|4.471668 4.689057 5.573836|46 ms
TEST_12|OK|6|6|4|6.491079 6.659940 7.324771|48 ms
TEST_13|OK|9|9|4|3.539737 3.827855 4.295249|46 ms
TEST_14|OK|0|0|4|3.757366 5.040243 5.445808|46 ms
TEST_15|OK|1|1|4|2.492252 2.553772 2.914340|40 ms
TEST_16|OK|5|5|4|6.364414 6.589044 6.859559|45 ms
TEST_17|OK|9|9|3|4.648328 4.819930 5.145173|45 ms
TEST_18|OK|7|7|4|4.646653 4.912968 5.198533|45 ms
TEST_19|OK|3|3|4|7.472582 8.427665 8.510137|48 ms
TEST_20|OK|4|4|4|3.962933 4.053201 4.342028|44 ms
TEST_21|OK|9|9|4|4.302878 4.501354 4.587317|46 ms
TEST_22|OK|6|6|4|4.875119 5.055512 5.567209|43 ms
TEST_23|OK|6|6|4|5.692452 6.024515 6.967512|43 ms
TEST_24|OK|5|5|4|5.240033 5.375215 5.675913|44 ms
TEST_25|NOK|4|9|3|4.563778 4.648925 5.511833|43 ms
TEST_26|OK|0|0|4|6.442389 6.469933 6.721232|49 ms
TEST_27|OK|7|7|4|4.105380 4.181578 4.868354|43 ms
TEST_28|OK|4|4|4|4.515115 4.520179 4.690135|49 ms
TEST_29|OK|0|0|4|5.284938 5.307717 5.608134|46 ms
TEST_30|OK|1|1|4|2.466978 2.603555 2.854266|41 ms
TEST_31|OK|3|3|4|5.452100 5.549013 6.133506|43 ms
TEST_32|OK|1|1|4|2.151648 2.384459 2.846194|41 ms
TEST_33|OK|3|3|4|4.918635 5.035085 5.550109|45 ms
TEST_34|OK|4|4|3|7.719063 7.832345 8.061908|46 ms
TEST_35|OK|7|7|4|3.552805 4.055440 4.580088|44 ms
TEST_36|OK|2|2|4|7.406676 7.488821 7.697243|47 ms
TEST_37|OK|7|7|4|5.236089 5.252931 5.429801|45 ms
TEST_38|OK|1|1|4|1.612366 1.676896 2.150193|41 ms
TEST_39|OK|2|2|3|6.289125 6.716333 6.823007|45 ms
TEST_40|OK|1|1|4|2.875005 2.906364 3.135262|42 ms
TEST_41|OK|1|1|4|2.218440 2.228404 2.641743|41 ms
TEST_42|OK|7|7|4|4.043702 4.556346 4.812568|42 ms
TEST_43|OK|4|4|4|4.812052 5.024737 5.214409|44 ms
TEST_44|NOK|2|1|4|6.309667 6.536674 6.619574|43 ms
TEST_45|NOK|3|5|1|5.618592 5.895691 6.164666|43 ms
TEST_46|OK|5|5|3|5.729671 5.911932 5.970106|44 ms
TEST_47|OK|1|1|4|2.972942 3.402625 3.793434|43 ms
TEST_48|OK|2|2|4|5.145636 5.572339 6.310497|45 ms
TEST_49|OK|4|4|4|5.951755 6.229398 6.322020|46 ms
TEST_50|OK|4|4|4|5.591473 5.768738 5.813098|45 ms
KNearestNeighbors::runTest(): TEST_DONE:
        k=3
        training_data=5000
        test_data=50
        error=3
        success=47
        pre_test=2
        test=2414
        post_test=1
KNearestNeighbors::runTest(): out
main(): ...test done, KNearestNeighbors::runTest() [2475 ms]
main(): out
```

## EXAMPLE_KMC

<span style="color: red;">As mentioned above, my implementation of K-Means Clustering seems to have flaws. Since I am new to this, am still discovering as I progress.</span>

## Parameters of main()

|Name|Description|
|:---|:---|
|path_train_images|interpreted as string; path to MNIST binary file with training image data|
|path_train_labels|interpreted as string; path to training file with labels|
|path_test_images|interpreted as string; path to MNIST binary file with test image data|
|path_test_labels|interpreted as string; path to test file with labels|
|train_count|interpretd as integer; number of records from training data to consider for calculations, defaults to 20000|
|train_exit_on_percentage|interpreted as double; exit training when percentage change falls below this threshold, defaults to 0.002|
|train_exit_on_unchanged|interpreted as integer; exit training when total distance / error is unchanged on this many consecutive iterations, defaults to 3|
|train_exit_on_max_iterations|interpred as integer; exit training after this many iterations, defaults to 50|
|k|interpred as integer; number of clusters, defaults to 10|
|test_count|interpreted as integer; number of records from test data to test, defaults to 50|
|output_mode|interpreted as string; defaults to "delimited", range: "delimited", "pretty"|

## Arguments of the run
5 of the 11 arguments provided via command line. For the remaining 6, default values are used.
This is the command line:
```
.\EXAMPLE_KMC.exe path_train_images=..\..\..\MNIST_DATA\train-images.idx3-ubyte path_train_labels=..\..\..\MNIST_DATA\train-labels.idx1-ubyte path_test_images=..\..\..\MNIST_DATA\t10k-images.idx3-ubyte path_test_labels=..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte train_count=25000
```
The check routine passed as a lambda expression to an instance of `ArgumentHandler` (see arg.hpp), assigns and logs the defaulted values:
```
argument check: setting 'train_exit_on_percentage' to default 0.002
argument check: setting 'train_exit_on_unchanged' to default 3
argument check: setting 'train_exit_on_max_iterations' to default 50
argument check: setting 'k' to default 10
argument check: setting 'test_count' to default 50
argument check: setting 'output_mode' to default 'delimited'
```
This is the corresponding code from `main_kmc.cpp`:
```
int main(int argument_count, char** arguments)
{
  std::cout << "main(): in" << std::endl;

  auto myCheck =
  [](std::array<ArgNameValue, MAX_COUNT_ARGS>& args)
    {
      //
      // check arguments
      //

      if (args[TRAIN_COUNT].value.length() == 0)
      {
        args[TRAIN_COUNT].value = "20000";
        std::cout << "argument check: setting \'" << args[TRAIN_COUNT].name << "\' to default 20000\n";
      }

      if (args[TRAIN_EXIT_PERCENT].value.length() == 0)
      {
        args[TRAIN_EXIT_PERCENT].value = "0.002";
        std::cout << "argument check: setting \'" << args[TRAIN_EXIT_PERCENT].name << "\' to default 0.002\n";
      }

      if (args[TRAIN_EXIT_UNCHANGED].value.length() == 0)
      {
        args[TRAIN_EXIT_UNCHANGED].value = "3";
        std::cout << "argument check: setting \'" << args[TRAIN_EXIT_UNCHANGED].name << "\' to default 3\n";
      }

      if (args[TRAIN_EXIT_ITERATIONS].value.length() == 0)
      {
        args[TRAIN_EXIT_ITERATIONS].value = "50";
        std::cout << "argument check: setting \'" << args[TRAIN_EXIT_ITERATIONS].name << "\' to default 50\n";
      }

      if (args[K].value.length() == 0)
      {
        args[K].value = "10";
        std::cout << "argument check: setting \'" << args[K].name << "\' to default 10\n";
      }

      if (args[TEST_COUNT].value.length() == 0)
      {
        args[TEST_COUNT].value = "50";
        std::cout << "argument check: setting \'" << args[TEST_COUNT].name << "\' to default 50\n";
      }

      if (args[OUTPUT_MODE].value.length() == 0)
      {
        args[OUTPUT_MODE].value = "delimited";
        std::cout << "argument check: setting \'" << args[OUTPUT_MODE].name << "\' to default \'delimited\'\n";
      }
    };

  ArgumentHandler argument_handler
  (
    argument_count
    , arguments
    , std::move(ARG_NAME)
    , myCheck
  );
```
|Name|Value|
|:---|:---|
|path_train_images|..\\..\\..\MNIST_DATA\train-images.idx3-ubyte|
|path_train_labels|..\\..\\..\MNIST_DATA\train-labels.idx1-ubyte|
|path_test_images|..\\..\\..\MNIST_DATA\t10k-images.idx3-ubyte|
|path_test_labels|..\\..\\..\MNIST_DATA\t10k-labels.idx1-ubyte|
|train_count|25000|
## Output of the run, Windows release build
```
main(): in
argument check: setting 'train_exit_on_percentage' to default 0.002
argument check: setting 'train_exit_on_unchanged' to default 3
argument check: setting 'train_exit_on_max_iterations' to default 50
argument check: setting 'k' to default 10
argument check: setting 'test_count' to default 50
argument check: setting 'output_mode' to default 'delimited'
main(): arg_0: path_train_images|..\..\..\MNIST_DATA\train-images.idx3-ubyte
main(): arg_1: path_train_labels|..\..\..\MNIST_DATA\train-labels.idx1-ubyte
main(): arg_2: path_test_images|..\..\..\MNIST_DATA\t10k-images.idx3-ubyte
main(): arg_3: path_test_labels|..\..\..\MNIST_DATA\t10k-labels.idx1-ubyte
main(): arg_4: train_count|25000
main(): arg_5: train_exit_on_percentage|0.002
main(): arg_6: train_exit_on_unchanged|3
main(): arg_7: train_exit_on_max_iterations|50
main(): arg_8: k|10
main(): arg_9: test_count|50
main(): arg_10: output_mode|delimited
main(): reading 4 files: training images, training labels, test images, test labels...
main(): ...4 files read [38 ms]
main(): instantiating kmc object with normalized training data
KMeansClustering::KMeansClustering(): in
#|CLST_ASSIGN|PCT_CHG|ERR|ERR_MIN
0|2249|100.000000|1684432.145161|0.000000
1|3385|36.832385|1064015.619749|1684432.145161
2|3377|2.250549|1040069.424754|1064015.619749
3|3386|1.378485|1025732.224154|1040069.424754
4|3374|0.895612|1016545.638372|1025732.224154
5|3417|0.464446|1011824.336303|1016545.638372
6|3460|0.337609|1008408.329129|1011824.336303
7|3412|0.309595|1005286.344544|1008408.329129
8|3427|0.179100|1003485.877517|1005286.344544
9|3447|0.132633|1002154.922516|1003485.877517
10|3450|0.131979|1000832.285361|1002154.922516
11|3481|0.135670|999474.458706|1000832.285361
12|3474|0.145104|998024.181999|999474.458706
13|3448|0.157009|996457.192865|998024.181999
14|3500|0.172241|994740.881060|996457.192865
15|3475|0.181719|992933.243499|994740.881060
16|3485|0.155836|991385.899186|992933.243499
17|3508|0.120760|990188.706112|991385.899186
18|3606|0.090340|989294.168630|990188.706112
19|3564|0.053608|988763.831411|989294.168630
20|3520|0.035297|988414.830557|988763.831411
21|3503|0.024555|988172.127203|988414.830557
22|3552|0.013955|988034.229193|988172.127203
23|3554|0.009549|987939.880808|988034.229193
24|3457|0.010329|987837.838546|987939.880808
25|3521|0.011011|987729.071418|987837.838546
26|3486|0.009302|987637.189222|987729.071418
27|3506|0.008608|987552.170760|987637.189222
28|3463|0.010221|987451.229201|987552.170760
29|3520|0.012984|987323.020294|987451.229201
30|3451|0.022773|987098.178136|987323.020294
31|3508|0.028190|986819.917832|987098.178136
32|3513|0.028073|986542.885843|986819.917832
33|3489|0.030113|986245.808552|986542.885843
34|3481|0.023956|986009.539073|986245.808552
35|3509|0.014895|985862.670392|986009.539073
36|3575|0.010097|985763.128600|985862.670392
37|3626|0.007300|985691.166779|985763.128600
38|3581|0.006505|985627.046269|985691.166779
39|3543|0.005613|985571.719787|985627.046269
40|3573|0.005485|985517.657375|985571.719787
41|3529|0.005113|985467.270416|985517.657375
42|3515|0.003778|985430.038023|985467.270416
43|3520|0.002264|985407.726018|985430.038023
44|3493|0.001550|985392.451133|985407.726018
KMeansClustering::KMeansClustering(): i_44: EXIT_CONDITION_PERCENTAGE_CHANGE
KMeansClustering::KMeansClustering(): out 156634 ms
main(): kmc object instantiated [156966 ms]
main(): running test with normalized test data
KMeansClustering::runTest(): in, clusters:10
KMeansClustering::runTest(): TEST_START:
        test_data=50
Test|Status|Expected|Result|Distance
TEST_1|OK|7|1|5.866963
TEST_2|OK|2|8|8.155932
TEST_3|OK|1|2|4.219680
TEST_4|OK|0|7|6.405600
TEST_5|OK|4|3|5.283593
TEST_6|OK|1|2|3.778854
TEST_7|OK|4|1|6.407001
TEST_8|OK|9|3|6.660697
TEST_9|OK|5|3|8.018097
TEST_10|OK|9|0|5.320407
TEST_11|OK|0|7|6.256046
TEST_12|OK|6|3|7.910142
TEST_13|OK|9|1|4.557625
TEST_14|OK|0|7|6.462813
TEST_15|OK|1|9|3.890771
TEST_16|OK|5|8|6.900473
TEST_17|OK|9|3|5.644959
TEST_18|OK|7|1|6.598965
TEST_19|OK|3|8|8.418439
TEST_20|OK|4|1|5.267378
TEST_21|OK|9|0|4.987625
TEST_22|OK|6|6|5.761994
TEST_23|OK|6|3|7.034838
TEST_24|OK|5|5|5.972072
TEST_25|OK|4|3|4.999206
TEST_26|OK|0|7|6.884858
TEST_27|OK|7|3|5.919763
TEST_28|OK|4|3|5.546248
TEST_29|OK|0|7|6.809737
TEST_30|OK|1|9|4.284295
TEST_31|OK|3|8|6.319320
TEST_32|OK|1|9|4.402950
TEST_33|OK|3|8|5.970411
TEST_34|OK|4|6|8.046195
TEST_35|OK|7|0|5.637288
TEST_36|OK|2|4|7.258497
TEST_37|OK|7|1|6.153517
TEST_38|OK|1|9|3.857772
TEST_39|OK|2|9|6.411729
TEST_40|OK|1|9|3.996779
TEST_41|OK|1|9|3.894342
TEST_42|OK|7|1|5.330076
TEST_43|OK|4|1|5.579770
TEST_44|OK|2|2|6.518210
TEST_45|OK|3|9|5.870803
TEST_46|OK|5|8|5.789528
TEST_47|OK|1|9|6.137631
TEST_48|OK|2|4|6.325936
TEST_49|OK|4|3|7.084274
TEST_50|OK|4|3|5.639464
KMeansClustering::runTest(): TEST_DONE:
        test_data=50
        error=0
        success=50
        test=25
KMeansClustering::runTest(): out
main(): test done, KMeansClustering::runTest() [86 ms]
main(): out
```
