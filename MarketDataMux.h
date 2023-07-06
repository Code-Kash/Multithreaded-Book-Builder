#ifndef MARKETDATAMUX_H
#define MARKETDATAMUX_H

#include <vector>
#include <iostream>
#include <string>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <filesystem>
#include "NonThreaded.h"

class MarketDataMux {
public:
    MarketDataMux();
    ~MarketDataMux();

    void muxData(const std::vector<std::string>& symbols);
    void writeToFile(const std::string& outputPath);

private:
    void processFile(const std::string& symbol);
    void processOrders();

    sortedOrders_byTimethenSymbol heap;

    std::deque<Order> ordersQueue;
    std::mutex mtx;
    std::condition_variable cv;
    std::atomic<int> filesToProcess;
    std::vector<std::thread> workers;
    std::atomic<bool> allFilesProcessed;
};

#endif
