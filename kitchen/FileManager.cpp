#include "FileManager.h"

#include <fstream>
#include <sstream>

FileManager::FileManager(const std::string& filePath) : filePath_(filePath) {}

std::vector<Order> FileManager::loadOrders() const {
    std::vector<Order> orders;
    std::ifstream input(filePath_);

    if (!input.is_open()) {
        return orders;
    }

    std::string line;
    while (std::getline(input, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string idText;
        std::string product;
        std::string status;

        if (!std::getline(ss, idText, '|')) {
            continue;
        }
        if (!std::getline(ss, product, '|')) {
            continue;
        }
        if (!std::getline(ss, status, '|')) {
            continue;
        }

        int id = 0;
        std::stringstream idStream(idText);
        idStream >> id;
        if (id <= 0) {
            continue;
        }

        orders.push_back(Order(id, product, status));
    }

    return orders;
}

void FileManager::saveOrders(const std::vector<Order>& orders) const {
    std::ofstream output(filePath_, std::ios::trunc);

    for (const auto& order : orders) {
        output << order.getId() << "|" << order.getProduct() << "|" << order.getStatus() << "\n";
    }
}
