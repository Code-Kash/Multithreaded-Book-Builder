cmake_minimum_required(VERSION 3.12)
project(SortOrders VERSION 1.0.0) 

set(CMAKE_CXX_STANDARD 20) 

add_executable(SortOrders
    src/main.cpp
    src/NonThreaded.cpp
    src/MarketDataMux.cpp
)

target_include_directories(SortOrders PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/includes
)