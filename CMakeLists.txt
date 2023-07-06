cmake_minimum_required(VERSION 3.12)
project(SortOrders VERSION 1.0.0) 

set(CMAKE_CXX_STANDARD 20) 

add_executable(SortOrders
    main.cpp
    NonThreaded.cpp
    MarketDataMux.cpp
)