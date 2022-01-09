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

Result MongoDB::updateUserFieldById(const std::string& id, const std::string& fieldName, const std::string& fieldValue) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;
        auto value = m_connection->collections.user.find_one(document {} << "id" << id << finalize);
        if (not value) {
            result.message = "User id " + id + " is not found";
            return result;
        }

        m_connection->collections.user.update_one(document {} << "id" << id << finalize, document {} << "$set" << open_document << fieldName << fieldValue << close_document << finalize);
        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::removeBookByUserId(const std::string& id, const std::string& bookId) noexcept {
    Result result;
    result.ok = false;
    try {
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        book::User user;
        result = getUserById(id, user);
        if (not result.ok) {
            result.ok = false;
            result.message = "User " + id + " doesnt exist";
            return result;
        }

        auto value = m_connection->collections.book.delete_one(document {} << "id" << bookId << finalize);
        if (not value) {
            result.message = "Book id " + bookId + " is not found";
            return result;
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::addBookByUserId(const std::string& userId, book::Book& book, std::string& bookId) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto value = m_connection->collections.user.find_one(document {} << "id" << userId << finalize);
        if (not value) {
            result.message = "User id doesnt exist";
            return result;
        }

        if (not book.IsInitialized()) {
            result.ok = false;
            result.message = "Book must be initialized";
            return result;
        }
        if (book.name().empty()) {
            result.ok = false;
            result.message = "User must have a name";
        }
        if (book.authors().empty()) {
            result.ok = false;
            result.message = "Book must have authors";
            return result;
        }

        bookId = generateId(book.name() + userId + book.authors());
        book.set_user_id(userId);
        m_connection->collections.user.insert_one(MessageToBsonDocumentValue(book));

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::getBooksByUserId(std::string const& userId, std::vector<book::Book>& books) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto cursor = m_connection->collections.book.find(document {} << "userId" << userId << finalize);

        for (auto doc : cursor) {
            book::Book newBook;
            BsonDocumentValueToMessage<book::Book>(bsoncxx::from_json(bsoncxx::to_json(doc)));
            books.push_back(newBook);
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}
