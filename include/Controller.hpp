//
// Created by alisher on 1/9/22.
//

#pragma once

#include <drogon/CacheMap.h>
#include <drogon/HttpController.h>
#include <memory>
#include <string>
#include <Database.hpp>
using drogon::HttpRequest;
using drogon::HttpResponse;
using drogon::HttpRequestPtr;
using drogon::HttpStatusCode;
using drogon::HttpResponsePtr;
using drogon::app;
using drogon::Get;
using drogon::Put;
using drogon::Post;
using drogon::Options;

namespace api {
namespace v1 {

    class Controller : public drogon::HttpController<Controller> {
       private:
        using Callback = std::function<void(HttpResponsePtr const&)>;
        using string = std::string;

       public:
        METHOD_LIST_BEGIN
        METHOD_ADD(Controller::getUserList, "/users", Get, Options);
        METHOD_ADD(Controller::getBookList, "/books", Get, Options);
        METHOD_ADD(Controller::getUserBookList, "/books/{1}", Get, Options);
        METHOD_LIST_END

        void getUserList(HttpRequestPtr const& req, Callback&& callback);
        void getBookList(HttpRequestPtr const& req, Callback&& callback);
        void getUserBookList(HttpRequestPtr const& req, Callback&& callback, std::string&& userId);

       private:
        MongoDB::Ptr getMongo();
    };
}// namespace v1
}// namespace api
