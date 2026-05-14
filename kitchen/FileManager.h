#pragma once

#include <string>
#include <vector>

#include "Order.h"

class FileManager {
public:
    explicit FileManager(const std::string& filePath);

    std::vector<Order> loadOrders() const;
    void saveOrders(const std::vector<Order>& orders) const;

private:
    std::string filePath_;
};
