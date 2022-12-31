cmake_minimum_required(VERSION 3.24)
project(Midterm_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(Midterm_Project
        CSVReader.cpp
        CSVReader.h
        main.cpp
        MerkelMain.cpp
        MerkelMain.h
        OrderBook.cpp
        OrderBook.h
        OrderBookEntry.cpp
        OrderBookEntry.h
        Wallet.cpp
        Wallet.h)
