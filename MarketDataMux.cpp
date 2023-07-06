#include "MarketDataMux.h"
#include <fstream>

MarketDataMux::MarketDataMux()
    : filesToProcess(0), allFilesProcessed(false) {
}

MarketDataMux::~MarketDataMux() {
}

void MarketDataMux::muxData(const std::vector<std::string>& symbols) {
    filesToProcess = symbols.size();

    std::vector<std::jthread> threads; //jthreads for STL threads with RAII
    threads.reserve(symbols.size() + 1);  // Reserve space for threads

    std::jthread ordersThread(&MarketDataMux::processOrders, this); //create a thread to process orders
    threads.push_back(std::move(ordersThread)); //move the thread into the vector (move semantics)

    for (const auto& symbol : symbols) {
        std::jthread fileThread(&MarketDataMux::processFile, this, symbol); //create a thread to process each file
        threads.push_back(std::move(fileThread)); //move the thread into the vector (move semantics)
    }

    allFilesProcessed = true;
    cv.notify_all(); //notify the orders thread that all files have been processed
}


void MarketDataMux::processFile(const std::string& symbol) {
    // Open the stock file and check to see if file is openable
    std::ifstream inFile(symbol + ".txt");
    if (!inFile.is_open()) {
        std::cout << "Failed to open file: " << symbol + ".txt" << std::endl;
        return;
    }

    std::string line;
    bool skipFirstLine = true; //skip the first line

    // Iterate through each line in the file
    while (std::getline(inFile, line)) {
        if (skipFirstLine) {
            skipFirstLine = false;
            continue;
        }

        if (line.empty() || line == "...") { //skip empty lines and "..."
            continue;
        }

        // Parse each order
        Order order(line, symbol);
        {
            std::lock_guard<std::mutex> lock(mtx); //lock the mutex (write operation)
            ordersQueue.push_back(order);
        }
        cv.notify_one(); //notify the orders thread that a new order has been added
    }

    {
        std::lock_guard<std::mutex> lock(mtx); //lock the mutex (write operation)
        --filesToProcess;
    }
    cv.notify_one(); //notify the orders thread that a file has been processed
}

void MarketDataMux::processOrders() {
    std::unique_lock<std::mutex> lock(mtx); //lock the mutex (read operation)
    cv.wait(lock, [this]() { return filesToProcess == 0; }); //wait until all files have been processed

    while (true) {
        cv.wait(lock, [this]() { return !ordersQueue.empty(); }); //wait until there are orders to process

        Order order = ordersQueue.front();
        ordersQueue.pop_front();
        heap.push(order);

        if (ordersQueue.empty() && filesToProcess == 0) { //if there are no more orders to process and no more files to process, break
            break;
        }
    }

    cv.notify_all();
}

void MarketDataMux::writeToFile(const std::string& outputPath) {
    std::ofstream resultFile(outputPath, std::ios::out);
    if (!resultFile.is_open()) {
        std::cout << "Error: could not open file" << '\n';
        return;
    }

    while (!heap.empty()) {
        Order order = heap.top();
        heap.pop();
        resultFile << order.symbol << ',' << order.timestamp << ',' << order.price << ',' << order.size
                   << ',' << order.exchange << ',' << order.type << '\n';
    }
}
