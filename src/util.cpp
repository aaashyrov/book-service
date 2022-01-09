//
// Created by alisher on 1/9/22.
//
#include <util.hpp>

std::string generateId(const std::string& name) noexcept {
    auto nameHash = std::hash<std::string>()(name);
    auto id = std::hash<unsigned>()(nameHash);
    return std::to_string(id);
}
