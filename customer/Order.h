#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <ostream>

class Order
{
private:
    int id_;
    std::string product_;
    std::string status_;

public:
    Order();
    Order(int id, const std::string &product, const std::string &status);

    int getId() const;
    void setId(int id);

    std::string getProduct() const;
    void setProduct(const std::string &product);

    std::string getStatus() const;
    void setStatus(const std::string &status);

    friend std::ostream &operator<<(std::ostream &os, const Order &order);
};

#endif
