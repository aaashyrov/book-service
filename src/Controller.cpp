//
// Created by alisher on 1/9/22.
//

#include "Controller.hpp"

#include "util.hpp"

#include <Database.hpp>

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
            auto response = HttpResponse::newHttpJsonResponse(to_json(getUsersListResponse));
            callback(response);

            std::cout << "get user list done" << std::endl;
            return;
        }
        getUsersListResponse.mutable_status()->set_ok(true);
        getUsersListResponse.mutable_content()->CopyFrom(users);
        auto response = HttpResponse::newHttpJsonResponse(to_json(getUsersListResponse));
        callback(response);

        std::cout << "get user list done" << std::endl;
    }

    void Controller::getBookList(const HttpRequestPtr& req, Callback&& callback) {
        std::cout<<"get book list start"<<std::endl;

        book::Books books;
        auto result = getMongo()->getBooks(books);

        book::GetBooksListResponse getBooksListResponse;
        if (not result.ok) {
            getBooksListResponse.mutable_status()->set_ok(false);
            getBooksListResponse.mutable_status()->set_message(result.message);
            auto response = HttpResponse::newHttpJsonResponse(to_json(getBooksListResponse));
            callback(response);

            std::cout << "get book list done" << std::endl;
            return;
        }
        getBooksListResponse.mutable_status()->set_ok(true);
        getBooksListResponse.mutable_content()->CopyFrom(books);
        auto response = HttpResponse::newHttpJsonResponse(to_json(getBooksListResponse));
        callback(response);

        std::cout << "get book list done" << std::endl;
    }

    void Controller::getUserBookList(const HttpRequestPtr& req, Callback&& callback, std::string&& userId) {}
    MongoDB::Ptr Controller::getMongo() {
        auto plugin = std::make_unique<Database*>(app().getPlugin<Database>());
        return (*plugin)->db;
    }
}// namespace v1
}// namespace api
