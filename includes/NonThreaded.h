#pragma once

#ifndef NONTHREADED_H
#define NONTHREADED_H

#include <cstring>
#include <sstream>
#include <queue>
//Create the header interface for the non threaded solution

//Create an order struct that holds the data from each line
//This struct is aligned to 8 bytes to avoid false sharing
struct alignas(8) Order {
    std::string symbol;
    char timestamp[25]; //cstyle array because SSO is unavilable for std::string
    std::string price;
    std::string size;
    std::string exchange;
    std::string type;

    //default constructor
    Order() = default;

    //parameterized constructor
    Order(const std::string& line, const std::string& _symbol);

    //overload the < operator to compare by timestamp, then by symbol
    bool operator<(const Order& other) const;
};

//create a comparator for the priority queue
struct CompareOrder {
    bool operator()(const Order& lhs, const Order& rhs) const;
};

//create minheap priority queue
typedef std::priority_queue<Order, std::vector<Order>, CompareOrder> sortedOrders_byTimethenSymbol;

#endif //NONTHREADED_H