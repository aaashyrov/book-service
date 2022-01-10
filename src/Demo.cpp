//
// Created by alisher on 1/10/22.
//
#include <Demo.hpp>
#include <fstream>
#include <util.hpp>
void Demo::initAndStart(const Json::Value& config) {
    Param user, book;
    user.dbName = config["user_db_name"].asString();
    user.ipAddr = config["user_db_ip_addr"].asString();

    book.dbName = config["book_db_name"].asString();
    book.ipAddr = config["book_db_ip_addr"].asString();

    db = MongoDB::instance(book, user);

    using namespace book;
    bool isFill = config["fill"].asBool();
    if (not isFill) {
        std::cout << "db user: " << user.ipAddr << "/" << user.dbName << " book: " << book.ipAddr << "/" << book.dbName << " is initialized\n";
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

    std::cout << "db user: " << user.ipAddr << "/" << user.dbName << " book: " << book.ipAddr << "/" << book.dbName << " is initialized\n";
}

void Demo::shutdown() {
    for (const auto& userId : userIds) {
        db->removeBooksByUserId(userId);
        db->removeUserById(userId);
    }

    std::cout << "db uninitialized\n";
}
