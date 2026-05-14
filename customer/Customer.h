#ifndef CUSTOMER_H
#define CUSTOMER_H

#include <string>
#include "User.h"
#include "Order.h"

class FileManager;

class Customer : public User
{
public:
    explicit Customer(const std::string &name = "Customer");
    Order createOrder(int id, const std::string &product) const;
    bool placeOrder(class FileManager &fileManager, const std::string &product) const;
};

#endif
