//
// Created by alisher on 1/10/22.
//
#include <Demo.hpp>
#include <fstream>
#include <util.hpp>
void Demo::initAndStart(const Json::Value& config) {
    std::string ipAddress = config["db_ip_addr"].asString();
    std::string name = config["db_name"].asString();
    db = MongoDB::instance(ipAddress, name);

    using namespace book;
    bool isTestFill = config["testFill"].asBool();
    if (not isTestFill) {
        std::cout << "db " << ipAddress << "/" << name << " is initialized\n";
        return;
    }

    auto snapshotFile = config["snapshot"].asString();

    std::ifstream in {snapshotFile};

    if (not in.is_open()) {
        std::cerr << "snapshot file " << snapshotFile << "doesnt exist\n";
        return;
    }

    std::string str;
    std::string content;
    while (std::getline(in, str)) {
        content.append(str);
    }

    DBSnapshot snapshot;
    google::protobuf::util::JsonParseOptions parseOptions;
    parseOptions.ignore_unknown_fields = true;
    google::protobuf::util::JsonStringToMessage(content, &snapshot, parseOptions);

    for (auto user : snapshot.users().users()) {
        std::string id;
        db->addUser(user, id);
        userIds.push_back(id);
    }

    for (auto book : snapshot.books().books()) {
        std::string id;
        db->addBookByUserId(book.user_id(), book, id);
    }

    std::cout << "db " << ipAddress << "/" << name << " is initialized\n";
}

void Demo::shutdown() {
    for (const auto& userId : userIds) {
        db->removeBooksByUserId(userId);
        db->removeUserById(userId);
    }

    std::cout << "db uninitialized\n";
}
