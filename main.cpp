// Objective

// Organize 10k files into a single file.
// Each input file consists of all orders for a single symbol, ordered by timestamp
// Result file needs to order all of these orders by timestamp
//     If two orders have same timestamp, then order the orders by symbol alphabetical order
// Solution should be as efficient as possible

// Input

// 10,000 files, each representing a unique stock symbol, comprising of all orders for that symbol, in timestamp order

// Output

// Single file of all orders ordered by timestamp

// Algorithm

// Naive Approach
//     Iterate through all 10k files, and then through each line in each file and add order to a result file 
//     that sorts these orders by timestamp (if timestamps equal, then sort by symbol)

//     Iterating through all files (n) and then through each line (m, avg # of lines) and then sorting them in binary search / heap /
//     ordered_map (log n * m) will have a time complexity of O(n * m * log (n * m)), which is quite inefficient

//     Also, this time complexity assumes we can store all of the data in memory

//     What data structures will be needed? 
//         A struct that organizes the data from each line (symbol, timestamp, price, size, exchange, type) - named Order
//         A priority_queue (min heap) that holds all orders sorted by timestamp, and then by stock symbol - named sortedOrders_byTimethenSymbol

//     What functions will be needed?
//         A function that iterates through all files - this will be in main.cpp
//         A function that iterates through all lines - this will be in main.cpp
//         A function that pushes the read order into a minheap - named processLine declared in NonThreaded.h
//         A function that pops these values into a result file - this will be in main.cpp


// Optimized Approach 
//     Create a thread pool, the size of our max cores, that go through all the files, and then store the most recent unused order
//     in each currently accessed file in a thread-safe data structure and use this to output to results

//     We no longer iterate through all files, instead we use a thread pool of k (number of cores) so they all run (in a perfect simulation) 
//     in parallel. Thus, we iterate through each file (n) and each line (m) divided by the size of the thread pool (k) 
//     and and then sort them using a binary heap of size k (log k), then output them directly to a result file
//     This will create a time complexity of O((n * m / k) * log(k)), which will be significantly more efficient 

//     Realistically, this time complexity is inaccurate especially for large file size and small core size, and the time complexity
//     could easily devolve to O(n * m * log(k)) as the file size >> thread pool. So the above time complexity assumes perfect behavior. 

//     This approach also introduces many new problems to solve. Many writers (each thread) will be writing to a single data 
//     structure in concurrency, thus we have a race condition and need to organize the algorithm and data structures 
//     very carefully to avoid data races. We can use direct locking mechanisms to induce synchronization like lock guards or shared locks.
//     However, this will make the solution signficantly slower due to the locking mechanism of a mutex.

//     To make this truly efficient, a lock-free data structure will be needed to achieve true concurrency, however this is a very complex
//     data structure and will take a lot of time to implement. Thus, a lock-based data structure will be used instead.

//     What data structures will be needed?
//         A struct that organizes the data from each line (symbol, timestamp, price, size, exchange, type) - named Order
//         A mutex to lock the data structure when writing to it - named mtx
//         A condition variable to notify the threads when a new order has been added - named cv
//         An atomic counter to keep track of the number of files to process - named filesToProcess
//         A vector of threads to hold the thread pool - named workers
//         A bool to keep track of whether all files have been processed - named allFilesProcessed
//         A deque to hold the orders - named ordersQueue
//         A priority_queue (min heap) that holds all orders sorted by timestamp, and then by stock symbol - named sortedOrders_byTimethenSymbol

//     What functions will be needed?
//         A function that iterates through all files - this will be in main.cpp
//         A function that iterates through all lines - this will be in main.cpp
//         A function that acts as an entry point into the multithreaded environment - named muxData
//         A function that processes each file - named processFile
//         A function that processes each order - named processOrders
//         A function that writes the orders to a file - named writeToFile

//NOTE: These approaches will assume that the input data is valid and will not check for invalid data, as invalid data is not specified in the problem statement
//NOTE: And exception handling will negatively impact performance, so it will not be used

#include <iostream>
#include <vector>
#include "MarketDataMux.h"

int main() {
    // Create path to stocks folder
    std::string stocksFolderPath = "../../Stocks";
    
    // Check if the directory exists
    std::filesystem::path desiredPath(stocksFolderPath);

    if (!std::filesystem::exists(desiredPath)) {
        std::cout << "Directory not found: " << desiredPath << std::endl;
        return 1;
    }

    // Set the current path to the stocks folder
    std::filesystem::current_path(desiredPath);

    std::cout << "Current working directory: " << std::filesystem::current_path() << std::endl;

    // Create MarketDataMux object
    MarketDataMux marketDataMux;

    // Vector to hold symbols
    std::vector<std::string> symbols;

    // Iterate over each file in the Stocks folder
    for (const auto& stockFile : std::filesystem::directory_iterator(std::filesystem::current_path())) {
        // Skip non txt files
        if (stockFile.path().extension() == ".txt" && stockFile.path().filename() != "result.txt") {
            // Get the symbol without the extension
            std::string symbol = stockFile.path().stem().string();
            symbols.push_back(symbol);
        }
    }

    // Call the muxData function to merge the files
    marketDataMux.muxData(symbols);

    // Call the writeToFile function to write the merged data to the output file
    marketDataMux.writeToFile("result.txt");

    return 0;
}



