cmake_minimum_required(VERSION 3.24)
project(Midterm_Project)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

configure_file(20200601.csv 20200601.csv @ONLY)

add_executable(Midterm_Project
        CSVReader.cpp
        CSVReader.h
        main.cpp
        Bot.cpp
        Bot.h
        OrderBook.cpp
        OrderBook.h
        OrderBookEntry.cpp
        OrderBookEntry.h
        StringUtil.h StringUtil.cpp)
