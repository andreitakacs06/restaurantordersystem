#include "User.h"

User::User(const std::string &name)
    : name_(name)
{
}

User::~User()
{
}

std::string User::getName() const
{
    return name_;
}

void User::setName(const std::string &name)
{
    name_ = name;
}
