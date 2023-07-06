#include "NonThreaded.h"

bool Order::operator<(const Order& other) const {
    //compare by timestamp first
    int tsComp = strcmp(timestamp, other.timestamp);
    if (tsComp < 0) { return false; }
    if (tsComp > 0) { return true; } 

    //if timestamps are equal, compare by symbol
    return symbol > other.symbol; 
}

Order::Order(const std::string& line, const std::string& _symbol) {
    //set the symbol
    symbol = _symbol;

    //create a string stream from the line
    std::stringstream ss(line);

    //parse the timestamp one char at a time
    timestamp[0] = ' '; //add a space to the beginning of the timestamp
    for (int i = 1; i < 24; i++) {
        ss >> std::noskipws >> timestamp[i]; //noskipws means no skip whitespace
    }
    timestamp[24] = '\0'; //null terminate the timestamp

    ss.ignore(); //ignore the comma
    std::getline(ss, price, ',');
    std::getline(ss, size, ','); 
    std::getline(ss, exchange, ',');
    std::getline(ss, type, ',');
}

//overload the () operator to create a functor
bool CompareOrder::operator()(const Order& lhs, const Order& rhs) const {
    return lhs < rhs;
}