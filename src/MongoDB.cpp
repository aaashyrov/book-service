//
// Created by alisher on 1/9/22.
//

#include <MongoDB.hpp>
#include <util.hpp>

MongoDB::MongoDB(Param const& book, Param const& user) noexcept {
    m_bookConnection = std::make_unique<Connection>(mongocxx::uri("mongodb://" + book.ipAddr), book.dbName, "book");
    m_userConnection = std::make_unique<Connection>(mongocxx::uri("mongodb://" + user.ipAddr), user.dbName, "user");
}

Result MongoDB::addUser(book::User& user, std::string& id) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto value = m_userConnection->collection().find_one(document {} << "login" << user.login() << finalize);
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
        m_userConnection->collection().insert_one(MessageToBsonDocumentValue(user));

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
        auto value = m_userConnection->collection().find_one(document {} << "id" << id << finalize);

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

        m_bookConnection->collection().delete_many(document {} << "userId" << id << finalize);
        auto value = m_userConnection->collection().delete_one(document {} << "id" << id << finalize);
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
        auto value = m_userConnection->collection().find_one(document {} << "id" << id << finalize);
        if (not value) {
            result.message = "User id " + id + " is not found";
            return result;
        }

        m_userConnection->collection().update_one(document {} << "id" << id << finalize, document {} << "$set" << open_document << fieldName << fieldValue << close_document << finalize);
        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::removeBookById(const std::string& bookId) noexcept {
    Result result;
    result.ok = false;
    try {
        using bsoncxx::builder::stream::document;
        using bsoncxx::builder::stream::finalize;

        auto value = m_bookConnection->collection().delete_one(document {} << "id" << bookId << finalize);
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

        auto value = m_userConnection->collection().find_one(document {} << "id" << userId << finalize);
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

        if (bookId.empty()) { bookId = generateId(book.name() + userId + book.authors()); }
        book.set_id(bookId);
        m_bookConnection->collection().insert_one(MessageToBsonDocumentValue(book));

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::getBooksByUserId(std::string const& userId, book::Books& books) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto cursor = m_bookConnection->collection().find(document {} << "userId" << userId << finalize);

        for (auto doc : cursor) {
            auto newBook = BsonDocumentValueToMessage<book::Book>(bsoncxx::from_json(bsoncxx::to_json(doc)));
            books.mutable_books()->Add()->CopyFrom(newBook);
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::getBooks(book::Books& books) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto cursor = m_bookConnection->collection().find({});
        for (auto doc : cursor) {
            auto newBook = BsonDocumentValueToMessage<book::Book>(bsoncxx::from_json(bsoncxx::to_json(doc)));
            books.mutable_books()->Add()->CopyFrom(newBook);
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::removeBooksByUserId(const std::string& userId) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;
        m_bookConnection->collection().delete_many(document {} << "userId" << userId << finalize);

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::getUsers(book::Users& users) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;

        auto cursor = m_userConnection->collection().find({});
        for (auto doc : cursor) {
            auto newUser = BsonDocumentValueToMessage<book::User>(bsoncxx::from_json(bsoncxx::to_json(doc)));
            users.mutable_users()->Add()->CopyFrom(newUser);
        }

        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.ok = false;
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::getBookById(const std::string& bookId, book::Book& book) noexcept {
    Result result;
    result.ok = false;
    try {
        using namespace bsoncxx::builder::stream;
        auto value = m_bookConnection->collection().find_one(document {} << "id" << bookId << finalize);

        if (not value) {
            result.message = "Id " + bookId + " is not found";
            return result;
        }

        book = BsonDocumentValueToMessage<book::Book>(*value);
        result.ok = true;
        return result;
    } catch (std::exception& ex) {
        result.message = ex.what();
        return result;
    }
}

Result MongoDB::addReviewByBookId(std::string const& bookId, book::Review const& review) noexcept {
    using namespace book;
    Book book;
    auto result = getBookById(bookId, book);
    if (not result.ok) { return result; }

    User user;
    result = getUserById(review.user_id(), user);
    if (not result.ok) { return result; }

    book.mutable_reviews()->Add()->CopyFrom(review);

    result = removeBookById(bookId);
    if (not result.ok) { return result; }

    std::string id = bookId;
    addBookByUserId(review.user_id(), book, id);
}
