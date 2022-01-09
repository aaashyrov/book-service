//
// Created by alisher on 1/9/22.
//

#include <MongoDB.hpp>
#include <util.hpp>

struct Collections {
    mongocxx::collection book;
    mongocxx::collection user;
};

struct MongoDB::Connection {
    Connection(std::string const& ipAddress, std::string const& dbName) {
        mongocxx::instance instance {};
        uri = mongocxx::uri("mongodb://" + ipAddress);
        client = mongocxx::client {uri};
        collections.book = client[dbName]["book"];
        collections.user = client[dbName]["user"];
    }

    mongocxx::uri uri;
    Collections collections;
    mongocxx::client client;
};

MongoDB::MongoDB(std::string const& ip, std::string const& name) noexcept {
    m_connection = std::make_unique<Connection>(ip, name);
}

Result MongoDB::addUser(book::User& user, std::string& id) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto value = m_connection->collections.user.find_one(document {} << "login" << user.login() << finalize);
        if (value) {
            result.message = "User login already exists";
            return result;
        }

        if (not user.IsInitialized()) {
            result.ok = false;
            result.message = "User must be initialized";
            return result;
        }
        if (not user.has_birthday()) {
            result.ok = false;
            result.message = "User must have a birthday";
            return result;
        }
        if (user.name().empty()) {
            result.ok = false;
            result.message = "User must have a name";
        }
        if (user.lastname().empty()) {
            result.ok = false;
            result.message = "User must have a lastname";
            return result;
        }
        if (user.email().empty()) {
            result.ok = false;
            result.message = "User must have an email";
            return result;
        }
        if (user.tel().empty()) {
            result.ok = false;
            result.message = "User must have a telephone number";
            return result;
        }
        if (user.login().empty()) {
            result.ok = false;
            result.message = "User must have a login";
            return result;
        }

        id = generateId(user.lastname() + user.name() + user.login());
        user.set_id(id);
        m_connection->collections.user.insert_one(MessageToBsonDocumentValue(user));

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::getUserById(std::string const& id, book::User& user) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;
        auto value = m_connection->collections.user.find_one(document {} << "id" << id << finalize);

        if (not value) {
            result.message = "Id " + id + " is not found";
            return result;
        }

        user = BsonDocumentValueToMessage<book::User>(*value);
        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::removeUserById(std::string const& id) noexcept {
    Result result;
    result.ok = false;
    try {
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        book::User user;
        result = getUserById(id, user);
        if (not result.ok) {
            result.ok = true;
            return result;
        }

        for (const auto& bookId : user.book_ids()) {
            removeBookById(bookId);
        }

        auto value = m_connection->collections.user.delete_one(document {} << "id" << id << finalize);
        if (not value) {
            result.message = "User id " + id + " is not found";
            return result;
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::removeBookById(std::string const& id) noexcept {
    Result result;
    result.ok = false;
    try {
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        auto value = m_connection->collections.book.delete_one(document {} << "id" << id << finalize);
        if (not value) {
            result.message = "Book id " + id+ " is not found";
            return result;
        }

        if (value->deleted_count() <= 0) {
            result.message = "Book id " + id + " is not found";
            return result;
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.message = ex.what();
        return result;
    }
}
//
//Result MongoDB::updateUserFieldByLogin(const std::string& login, const std::string& fieldName, const std::string& fieldValue) noexcept {
//    Result result;
//    result.ok = false;
//    try {
//        using namespace bsoncxx::builder::stream;
//        auto value = m_connection->collections.user.find_one(document {} << "login" << login << finalize);
//        if (not value) {
//            result.message = "Login " + login + " is not found";
//            return result;
//        }
//
//        m_connection->collections.user.update_one(document {} << "login" << login << finalize, document {} << "$set" << open_document << fieldName << fieldValue << close_document << finalize);
//    } catch (std::exception& ex) {
//        result.ok = false;
//        result.message = ex.what();
//        return result;
//    }
//}
//
//Result MongoDB::updateUserFieldByLogin(const std::string& login, const std::string& fieldName, const std::vector<std::string>& vector) noexcept {
//    Result result;
//    result.ok = false;
//    try {
//        using namespace bsoncxx::builder::stream;
//        auto value = m_connection->collections.user.find_one(document {} << "login" << login << finalize);
//        if (not value) {
//            result.message = "Login " + login + " is not found";
//            return result;
//        }
//
//    } catch (std::exception& ex) {
//        result.ok = false;
//        result.message = ex.what();
//        return result;
//    }
//}
