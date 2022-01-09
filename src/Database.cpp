//
// Created by alisher on 1/10/22.
//
#include <Database.hpp>

void Database::initAndStart(const Json::Value& config) {
    std::string ipAddress = config["ip_addr"].asString();
    std::string name = config["name"].asString();
    db = MongoDB::instance(ipAddress, name);

    std::cout << "db " << ipAddress << "/" << name << " is initialized\n";
}

void Database::shutdown() {

}
