#include "FileManager.h"

#include <fstream>
#include <sstream>
#include <iostream>

FileManager::FileManager(const std::string &filename)
    : filename_(filename)
{
}

bool FileManager::loadOrders()
{
    orders_.clear();

    std::ifstream inputFile(filename_);
    if (!inputFile.is_open())
    {
        // If the file does not exist yet, create it empty and return success.
        std::ofstream createFile(filename_);
        return createFile.is_open();
    }

    std::string line;
    while (std::getline(inputFile, line))
    {
        if (line.empty())
        {
            continue;
        }

        std::stringstream ss(line);
        std::string idText;
        std::string product;
        std::string status;

        if (!std::getline(ss, idText, '|'))
        {
            continue;
        }
        if (!std::getline(ss, product, '|'))
        {
            continue;
        }
        if (!std::getline(ss, status))
        {
            continue;
        }

        int id = 0;
        try
        {
            id = std::stoi(idText);
        }
        catch (...)
        {
            continue;
        }

        orders_.emplace_back(id, product, status);
    }

    return true;
}

bool FileManager::appendOrder(const Order &order)
{
    std::ofstream outputFile(filename_, std::ios::app);
    if (!outputFile.is_open())
    {
        return false;
    }

    outputFile << order.getId() << "|"
               << order.getProduct() << "|"
               << order.getStatus() << "\n";

    if (!outputFile.good())
    {
        return false;
    }

    orders_.push_back(order);
    return true;
}

const std::vector<Order> &FileManager::getOrders() const
{
    return orders_;
}

int FileManager::getNextOrderId() const
{
    int highestId = 0;
    for (const auto &order : orders_)
    {
        if (order.getId() > highestId)
        {
            highestId = order.getId();
        }
    }
    return highestId + 1;
}
