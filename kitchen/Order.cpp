#include "Order.h"

Order::Order() : id_(0), product_(""), status_("") {}

Order::Order(int id, const std::string& product, const std::string& status)
    : id_(id), product_(product), status_(status) {}

int Order::getId() const {
    return id_;
}

const std::string& Order::getProduct() const {
    return product_;
}

const std::string& Order::getStatus() const {
    return status_;
}

void Order::setProduct(const std::string& product) {
    product_ = product;
}

void Order::setStatus(const std::string& status) {
    status_ = status;
}

std::ostream& operator<<(std::ostream& os, const Order& order) {
    os << "ID: " << order.getId() << " | "
       << "Product: " << order.getProduct() << " | "
       << "Status: " << order.getStatus();
    return os;
}
