# Simple-Hybesis-Algorithms
Educational demonstration of building simple algorithms under good practices developpment (cross-building, unit testing, ...)

# CMake - Cross-Building
The building relies on CMake, an Open-Source system that allows to easily build cross-plateform applications by generating native makefiles (cf. http://www.cmake.org/cmake/resources/software.html).

The current compilation has been fully tested under Win 7 (VS 11 x86/x64) and gcc.

## CMake - GTest - Unit Testing
Use the CMake 'BUILD_TESTING' option to automatically setup Unit Testing with google test.
It allows to very quickly get set up with C++ and google test:

- Clone GTest via GIT
- Setup the Unit Testings
- Compile GTest at build time

# Getting Started
## Build using CMake-GUI:

    Run CMake-GUI
    Select source/target directory
    Click on Configure the project
    Click on Generate the Solution files
    Open the solution file (if MSVC) and build or use make command.

## Build in Command-Line (UNIX):

    mkdir Simple-Hybesis-Algorithms-Build (Create Build directory)
    cd Simple-Hybesis-Algorithms-Build    (Open build director)
    ccmake ../Simple-Hybesis-Algorithms   (Configure the make given Source repository)
    make                                  (Make command, use -j parameter to use multicore)

## Run the tests:

The 'BUILD_TESTING' option will create executables that can be run directly from build directory.

E.g. to manually run the 'TestBasicSearch' Unit Test (using GTest):

    Simple-Hybesis-Algorithms-Build/Modules/Array/Testing/Debug/TestBasicSearch.exe  (Win)
    ./Simple-Hybesis-Algorithms-Build/Modules/Array/Testing/Debug/TestBasicSearch    (UNIX)

# Documentation
Please find the documentation here:
http://michaeljeulin-l.com/Projects/SHADocumentation/md__r_e_a_d_m_e.html

Note: an automatical CMake doc generation will be soon available.

# Current Available Algorithms
### Array
#### Searching
- **Binary Search:** Iteratively proceed a dichotomic search within a sorted vector on the first occurence of the key passed as parameter.
- **Kth Smallest / Biggest element - Order Statitstics:** Find the kth smallest/biggest element contained within [begin, end].
- **Maximal Distance:** Identify the two indexes of the array with the maximal distance.
- **Maximal M Elements:** Identify the m maximal/minimal values sorted in respectively decreasing increasing order.
- **Maximal Subarray:** Identify the subarray with the maximum/minimum sum. One of the problem resolved by this algorithm is:
"Given an array of gains/losses over time, find the period that represents the best/worst cummulative gain."

#### Sorting
- **MergeInplace:** Proceed a in place merge of two sequences of elements contained in [begin, middle[ and [middle, end[.
- **Partition-Exchange:** Proceed an in-place patitionning on the elements.
- **Quick Sort - Partition-Exchange Sort:** Proceed an in-place quick-sort on the elements.
- **Raddix Sort - LSD:** Proceed the Least Significant Digit Raddix sort, a non-comparative integer sorting algorithm.
