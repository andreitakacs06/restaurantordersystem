#include "Order.h"

Order::Order()
    : id_(0), product_(""), status_("Pending")
{
}

Order::Order(int id, const std::string &product, const std::string &status)
    : id_(id), product_(product), status_(status)
{
}

int Order::getId() const
{
    return id_;
}

void Order::setId(int id)
{
    id_ = id;
}

std::string Order::getProduct() const
{
    return product_;
}

void Order::setProduct(const std::string &product)
{
    product_ = product;
}

std::string Order::getStatus() const
{
    return status_;
}

void Order::setStatus(const std::string &status)
{
    status_ = status;
}

std::ostream &operator<<(std::ostream &os, const Order &order)
{
    // Display a single order in a clean, readable format.
    os << "Order ID: " << order.id_
       << " | Product: " << order.product_
       << " | Status: " << order.status_;
    return os;
}
