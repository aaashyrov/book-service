//
// Created by alisher on 1/9/22.
//

#pragma once

#include <Demo.hpp>
#include <drogon/CacheMap.h>
#include <drogon/HttpController.h>
#include <memory>
#include <string>

using drogon::app;
using drogon::Get;
using drogon::Put;
using drogon::Post;
using drogon::Options;

using drogon::HttpRequest;
using drogon::HttpResponse;
using drogon::HttpRequestPtr;
using drogon::HttpStatusCode;
using drogon::HttpResponsePtr;

namespace api {
namespace v1 {

    class Controller : public drogon::HttpController<Controller> {
       private:
        using Callback = std::function<void(HttpResponsePtr const&)>;

       public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Controller::getBook, "/book/{1}", Get, Options);
        METHOD_ADD(Controller::getUser, "/user/{1}", Get, Options);
        METHOD_ADD(Controller::getUserList, "/users", Get, Options);
        METHOD_ADD(Controller::getBookList, "/books", Get, Options);
        METHOD_ADD(Controller::getUserBookList, "/user/books/{1}", Get, Options);
        METHOD_ADD(Controller::addUserBook, "/user/book/add/{1}", Post, Options);
        METHOD_ADD(Controller::delBook, "/book/del/{1}", Post, Options);
        METHOD_LIST_END

       public:
        void getUserList(HttpRequestPtr const& req, Callback&& callback);
        void getBookList(HttpRequestPtr const& req, Callback&& callback);
        void getUser(HttpRequestPtr const& req, Callback&& callback, std::string&& userId);
        void getBook(HttpRequestPtr const& req, Callback&& callback, std::string&& bookId);
        void delBook(HttpRequestPtr const& req, Callback&& callback, std::string&& bookId);
        void addUserBook(HttpRequestPtr const& req, Callback&& callback, std::string&& userId);
        void getUserBookList(HttpRequestPtr const& req, Callback&& callback, std::string&& userId);

       private:
        MongoDB::Ptr getMongo();
    };
}// namespace v1
}// namespace api
