#pragma once

#include <string>

class User {
public:
    explicit User(const std::string& name);
    virtual ~User();

    const std::string& getName() const;

protected:
    std::string name_;
};
