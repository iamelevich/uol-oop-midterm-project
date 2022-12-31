#include "OrderBookEntry.h"

#include <utility>

OrderBookEntry::OrderBookEntry(double _price,
                               double _amount,
                               std::string _timestamp,
                               std::string _product,
                               OrderBookType _orderType,
                               std::string _username)
        : price(_price),
          amount(_amount),
          timestamp(std::move(_timestamp)),
          product(std::move(_product)),
          orderType(_orderType),
          username(std::move(_username)) {}

OrderBookType OrderBookEntry::stringToOrderBookType(const std::string &s) {
    if (s == "ask") {
        return OrderBookType::ask;
    }
    if (s == "bid") {
        return OrderBookType::bid;
    }
    return OrderBookType::unknown;
}
