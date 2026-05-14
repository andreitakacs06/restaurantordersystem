#include "User.h"

User::User(const std::string& name) : name_(name) {}

User::~User() = default;

const std::string& User::getName() const {
    return name_;
}
