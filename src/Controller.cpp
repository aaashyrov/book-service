//
// Created by alisher on 1/9/22.
//

#include "Controller.hpp"

#include "util.hpp"

#include <Demo.hpp>

namespace api {
namespace v1 {
    void Controller::getUserList(const HttpRequestPtr& req, Callback&& callback) {
        std::cout << "get user list start" << std::endl;
        book::Users users;
        auto result = getMongo()->getUsers(users);

        book::GetUsersListResponse getUsersListResponse;
        if (not result.ok) {
            getUsersListResponse.mutable_status()->set_ok(false);
            getUsersListResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(getUsersListResponse));
            callback(response);

            std::cout << "get user list done" << std::endl;
            return;
        }
        getUsersListResponse.mutable_status()->set_ok(true);
        getUsersListResponse.mutable_content()->CopyFrom(users);
        auto response = HttpResponse::newHttpJsonResponse(toJson(getUsersListResponse));
        callback(response);

        std::cout << "get user list done" << std::endl;
    }

    void Controller::getBookList(const HttpRequestPtr& req, Callback&& callback) {
        std::cout << "get book list start" << std::endl;

        book::Books books;
        auto result = getMongo()->getBooks(books);

        book::GetBooksListResponse getBooksListResponse;
        if (not result.ok) {
            getBooksListResponse.mutable_status()->set_ok(false);
            getBooksListResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(getBooksListResponse));
            callback(response);

            std::cout << "get book list done" << std::endl;
            return;
        }
        getBooksListResponse.mutable_status()->set_ok(true);
        getBooksListResponse.mutable_content()->CopyFrom(books);
        auto response = HttpResponse::newHttpJsonResponse(toJson(getBooksListResponse));
        callback(response);

        std::cout << "get book list done" << std::endl;
    }

    void Controller::getUser(const HttpRequestPtr& req, Controller::Callback&& callback, std::string&& userId) {
        std::cout << "get user start" << std::endl;

        book::User user;
        auto result = getMongo()->getUserById(userId, user);

        book::GetUserResponse getUserResponse;
        if (not result.ok) {
            getUserResponse.mutable_status()->set_ok(false);
            getUserResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(getUserResponse));
            callback(response);

            std::cout << "get user done" << std::endl;
            return;
        }
        getUserResponse.mutable_status()->set_ok(true);
        getUserResponse.mutable_user()->CopyFrom(user);
        auto response = HttpResponse::newHttpJsonResponse(toJson(getUserResponse));
        callback(response);

        std::cout << "get user done" << std::endl;
    }

    void Controller::getBook(const HttpRequestPtr& req, Controller::Callback&& callback, std::string&& bookId) {
        std::cout << "get book start" << std::endl;

        book::Book book;
        auto result = getMongo()->getBookById(bookId, book);

        book::GetUserResponse getUserResponse;
        if (not result.ok) {
            getUserResponse.mutable_status()->set_ok(false);
            getUserResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(getUserResponse));
            callback(response);

            std::cout << "get user done" << std::endl;
            return;
        }
        getUserResponse.mutable_status()->set_ok(true);
        getUserResponse.mutable_user()->CopyFrom(book);
        auto response = HttpResponse::newHttpJsonResponse(toJson(getUserResponse));
        callback(response);

        std::cout << "get book  done" << std::endl;
    }

    void Controller::getUserBookList(const HttpRequestPtr& req, Callback&& callback, std::string&& userId) {
        std::cout << "get user book list start" << std::endl;

        book::Books books;
        auto result = getMongo()->getBooksByUserId(userId, books);

        book::GetBooksListResponse getBooksListResponse;
        if (not result.ok) {
            getBooksListResponse.mutable_status()->set_ok(false);
            getBooksListResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(getBooksListResponse));
            callback(response);

            std::cout << "get user book list done" << std::endl;
            return;
        }
        getBooksListResponse.mutable_status()->set_ok(true);
        getBooksListResponse.mutable_content()->CopyFrom(books);
        auto response = HttpResponse::newHttpJsonResponse(toJson(getBooksListResponse));
        callback(response);

        std::cout << "get user book list done" << std::endl;
    }

    MongoDB::Ptr Controller::getMongo() {
        auto plugin = std::make_unique<Demo*>(app().getPlugin<Demo>());
        return (*plugin)->db;
    }

    void Controller::addUserBook(const HttpRequestPtr& req, Controller::Callback&& callback, std::string&& userId) {
        std::cout << "add user book start " << std::endl;

        using namespace book;
        std::string data {req->getBody()};

        Book book;
        google::protobuf::util::JsonParseOptions parseOptions;
        parseOptions.ignore_unknown_fields = true;
        google::protobuf::util::JsonStringToMessage(data, &book, parseOptions);

        AddUserBookResponse addUserBookResponse;

        std::string bookId;
        auto result = getMongo()->addBookByUserId(userId, book, bookId);
        if (not result.ok) {
            addUserBookResponse.mutable_status()->set_ok(false);
            addUserBookResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(addUserBookResponse));
            callback(response);

            std::cout << "add user book done " << std::endl;
            return;
        }

        addUserBookResponse.mutable_status()->set_ok(true);
        addUserBookResponse.set_id(bookId);
        auto response = HttpResponse::newHttpJsonResponse(toJson(addUserBookResponse));
        callback(response);
        std::cout << "add user book done " << std::endl;
    }

    void Controller::delBook(const HttpRequestPtr& req, Controller::Callback&& callback, std::string&& bookId) {
        std::cout << "del book start " << std::endl;

        using namespace book;

        DelBookResponse delBookResponse;

        auto result = getMongo()->removeBookById(bookId);
        if (not result.ok) {
            delBookResponse.mutable_status()->set_ok(false);
            delBookResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(toJson(delBookResponse));
            callback(response);

            std::cout << "del book done " << std::endl;
            return;
        }

        delBookResponse.mutable_status()->set_ok(true);
        delBookResponse.set_id(bookId);
        auto response = HttpResponse::newHttpJsonResponse(toJson(delBookResponse));
        callback(response);
        std::cout << "del book done " << std::endl;
    }

}// namespace v1
}// namespace api
