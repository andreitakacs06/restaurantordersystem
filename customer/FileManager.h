#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <iostream>
#include <string>
#include <vector>
#include "Order.h"

class FileManager
{
private:
    std::string filename_;
    std::vector<Order> orders_;

public:
    explicit FileManager(const std::string &filename);

    bool loadOrders();

    bool appendOrder(const Order &order);

    const std::vector<Order> &getOrders() const;

    int getNextOrderId() const;
};

template <typename T>
void displayContainerItems(const std::vector<T> &items)
{
    for (const auto &item : items)
    {
        std::cout << item << std::endl;
    }
}

#endif
